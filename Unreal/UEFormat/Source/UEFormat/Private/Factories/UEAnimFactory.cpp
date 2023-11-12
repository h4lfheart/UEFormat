
#include "Factories/UEAnimFactory.h"
#include "ComponentReregisterContext.h"
#include "SkeletalMeshAttributes.h"
#include "Widgets/Anim/UAnimImportOptions.h"
#include "Widgets/Anim/UAnimWidget.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Interfaces/IMainFrameModule.h"
#include "Misc/FeedbackContext.h"
#include "Misc/ScopedSlowTask.h"
#include "Readers/UEAnimReader.h"

UEAnimFactory::UEAnimFactory(const FObjectInitializer & ObjectInitializer): Super(ObjectInitializer)
{
	Formats.Add(TEXT("ueanim; UEANIM Animation File"));
	SupportedClass = UAnimSequence::StaticClass();
	bCreateNew = false;
	bEditorImport = true;
	SettingsImporter = CreateDefaultSubobject<UAnimImportOptions>(TEXT("Anim Options"));
}

UObject* UEAnimFactory::FactoryCreateFile(UClass* Class, UObject* Parent, FName Name, EObjectFlags Flags, const FString& Filename, const TCHAR* Params, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{
	FScopedSlowTask SlowTask(5, NSLOCTEXT("UEAnimFactory", "BeginReadUEAnimFile", "Reading UEAnim file"), true);
	if (Warn->GetScopeStack().Num() == 0)
	{
		SlowTask.MakeDialog(true);
	}
	SlowTask.EnterProgressFrame(0);

	UEAnimReader Data = UEAnimReader(Filename);
	if (!Data.Read()) return nullptr;

	//Ui
	if (SettingsImporter->bInitialized == false)
	{
		TSharedPtr<UAnimWidget> ImportOptionsWindow;
		TSharedPtr<SWindow> ParentWindow;
		if (FModuleManager::Get().IsModuleLoaded("MainFrame"))
		{
			IMainFrameModule& MainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
			ParentWindow = MainFrame.GetParentWindow();
		}

		TSharedRef<SWindow> Window = SNew(SWindow).Title(FText::FromString(TEXT("Animation Import Options"))).SizingRule(ESizingRule::Autosized);
		Window->SetContent
		(
			SAssignNew(ImportOptionsWindow, UAnimWidget).WidgetWindow(Window)
		);
		SettingsImporter = ImportOptionsWindow.Get()->Stun;
		FSlateApplication::Get().AddModalWindow(Window, ParentWindow, false);
		bImport = ImportOptionsWindow.Get()->ShouldImport();
		bImportAll = ImportOptionsWindow.Get()->ShouldImportAll();
		SettingsImporter->bInitialized = true;
	}

	UAnimSequence* AnimSequence = NewObject<UAnimSequence>(Parent, Data.Header.ObjectName.c_str(), Flags);
	IAnimationDataController& Controller = AnimSequence->GetController();
	USkeleton* Skeleton = SettingsImporter->Skeleton;

	AnimSequence->SetSkeleton(Skeleton);
	Controller.OpenBracket(FText::FromString("Importing UEAnim Animation"));
	Controller.InitializeModel();
	AnimSequence->ResetAnimation();

	Controller.SetFrameRate(FFrameRate(Data.FramesPerSecond, 1));
	Controller.SetNumberOfFrames(FFrameNumber(Data.NumFrames));

	FScopedSlowTask ImportTask(Data.Tracks.Num(), FText::FromString("Importing UEAnim Animation"));
	ImportTask.MakeDialog(false);

	for (auto i = 0; i < Data.Tracks.Num(); i++)
	{
		ImportTask.EnterProgressFrame();

		FName BoneName = Data.Tracks[i].TrackName.c_str();
		auto PosKeys = Data.Tracks[i].TrackPosKeys;
		auto RotKeys = Data.Tracks[i].TrackRotKeys;
		auto ScaleKeys = Data.Tracks[i].TrackScaleKeys;

		TArray<FVector3f> FinalPosKeys;
		TArray<FQuat4f> FinalRotKeys;
		TArray<FVector3f> FinalScaleKeys;
		FinalPosKeys.SetNum(Data.NumFrames);
		FinalRotKeys.SetNum(Data.NumFrames);
		FinalScaleKeys.SetNum(Data.NumFrames);

		FVector3f PrevPos = FVector3f::OneVector;
		FQuat4f PrevRot = FQuat4f::Identity;
		FVector3f PrevScale = FVector3f::OneVector;

		for (auto j = 0; j < Data.NumFrames; j++)
		{
			for (auto k = 0; k < PosKeys.Num(); k++)
			{
				auto Frame = PosKeys[k].Frame;
				if (Frame == j)
				{
					FinalPosKeys[j] = PosKeys[k].VectorValue;
					PrevPos = PosKeys[k].VectorValue;
				}
				else
				{
					FinalPosKeys[j] = PrevPos;
				}
			}
			for (auto k = 0; k < RotKeys.Num(); k++)
			{
				auto Frame = RotKeys[k].Frame;
				if (Frame == j)
				{
					FinalRotKeys[j] = RotKeys[k].QuatValue;
					PrevRot = RotKeys[k].QuatValue;
				}
				else
				{
					FinalRotKeys[j] = PrevRot;
				}
			}
			for (auto k = 0; k < ScaleKeys.Num(); k++)
			{
				auto Frame = ScaleKeys[k].Frame;
				if (Frame == j)
				{
					FinalScaleKeys[j] = ScaleKeys[k].VectorValue;
					PrevScale = ScaleKeys[k].VectorValue;
				}
				else
				{
					FinalScaleKeys[j] = PrevScale;
				}
			}
		}
		Controller.AddBoneCurve(BoneName);
		Controller.SetBoneTrackKeys(BoneName, FinalPosKeys, FinalRotKeys, FinalScaleKeys);
	}
	
	if (!bImportAll)
	{
		SettingsImporter->bInitialized = false;
	}

	AnimSequence->GetController().NotifyPopulated();
	AnimSequence->GetController().CloseBracket();
	AnimSequence->PostEditChange();

	FAssetRegistryModule::AssetCreated(AnimSequence);
	FGlobalComponentReregisterContext RecreateComponents;

	return AnimSequence;
}
