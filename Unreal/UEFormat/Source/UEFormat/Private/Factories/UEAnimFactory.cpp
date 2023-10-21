
#include "Factories/UEAnimFactory.h"
#include "ComponentReregisterContext.h"
#include "SkeletalMeshAttributes.h"
#include "Widgets/EAnimImportOptions.h"
#include "Widgets/UEAnimWidget.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Interfaces/IMainFrameModule.h"
#include "Misc/FeedbackContext.h"
#include "Misc/ScopedSlowTask.h"
#include "Readers/UEAnimReader.h"

/* UTextAssetFactory structors
 *****************************************************************************/

	UEAnimFactory::UEAnimFactory(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	Formats.Add(TEXT("ueanim; UEANIM Animation File"));
	SupportedClass = UAnimSequence::StaticClass();
	bCreateNew = false;
	bEditorImport = true;
	SettingsImporter = CreateDefaultSubobject<UEAnimImportOptions>(TEXT("Anim Options"));
}

/* UFactory overrides
 *****************************************************************************/

UObject* UEAnimFactory::FactoryCreateFile(UClass* Class, UObject* Parent, FName Name, EObjectFlags Flags, const FString& Filename, const TCHAR* Params, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{

	/* Ui
	*****************************************************************************/

	FScopedSlowTask SlowTask(5, NSLOCTEXT("UEAnimFactory", "BeginReadUEAnimFile", "Reading UEAnim file"), true);
	if (Warn->GetScopeStack().Num() == 0)
	{
		// We only display this slow task if there is no parent slowtask, because otherwise it is redundant and doesn't display any relevant information on the progress.
		// It is primarly used to regroup all the smaller import sub-tasks for a smoother progression.
		SlowTask.MakeDialog(true);
	}
	SlowTask.EnterProgressFrame(0);

	// picker
	if (SettingsImporter->bInitialized == false)
	{
		TSharedPtr<UEAnimWidget> ImportOptionsWindow;
		TSharedPtr<SWindow> ParentWindow;
		if (FModuleManager::Get().IsModuleLoaded("MainFrame"))
		{
			IMainFrameModule& MainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
			ParentWindow = MainFrame.GetParentWindow();
		}

		TSharedRef<SWindow> Window = SNew(SWindow)
			.Title(FText::FromString(TEXT("UEAnim Import Options")))
			.SizingRule(ESizingRule::Autosized);
		Window->SetContent
		(
			SAssignNew(ImportOptionsWindow, UEAnimWidget)
			.WidgetWindow(Window)
		);
		SettingsImporter = ImportOptionsWindow.Get()->Stun;
		FSlateApplication::Get().AddModalWindow(Window, ParentWindow, false);
		bImport = ImportOptionsWindow.Get()->ShouldImport();
		bImportAll = ImportOptionsWindow.Get()->ShouldImportAll();
		SettingsImporter->bInitialized = true;
	}

	UEAnimReader Data = UEAnimReader(Filename);
	if (!Data.Read()) return nullptr;

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

		for (auto j = 0; j < PosKeys.Num(); i++)
		{
			float Time = PosKeys[i].Frame / Data.NumFrames;
			auto Value = PosKeys[i].VectorValue;
		}

		Controller.AddBoneCurve(BoneName);
		//Controller.SetBoneTrackKeys(BoneName, FinalPosKeys, FinalRotKeys, FinalScaleKeys);

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

//test
void UEAnimFactory::InterpolateVectorKeys(const TArray<FVectorKey>& Keys, TArray<FVector3f>& FinalKeys, int FrameIndex, float DataFramesPerSecond, FVector3f DefaultValue)
{
	float PrevTime = 0.0f;
	FVector3f PrevValue = DefaultValue;
	const float CurrentTime = FrameIndex / DataFramesPerSecond;

	for (int KeyIndex = 0; KeyIndex < Keys.Num(); KeyIndex++)
	{
		const float Time = Keys[KeyIndex].Frame / DataFramesPerSecond;
		const FVector3f Value = Keys[KeyIndex].VectorValue;

		if (Time >= CurrentTime)
		{
			const float DeltaTime = Time - PrevTime;
			const float Alpha = (DeltaTime > 0.0f) ? (CurrentTime - PrevTime) / DeltaTime : 0.0f;
			FinalKeys[FrameIndex] = FMath::Lerp(PrevValue, Value, Alpha);
			break;
		}
		PrevTime = Time;
		PrevValue = Value;
	}
}
