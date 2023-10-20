
#include "Factories/UEAnimFactory.h"
#include "ComponentReregisterContext.h"
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

	FScopedSlowTask SlowTask(5, NSLOCTEXT("UEAnimFactory", "BeginReadUEAnimFile", "Opening UEAnim file."), true);
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

	auto AnimSequence = NewObject<UAnimSequence>(Parent, Name, Flags);
	
	USkeleton* Skeleton = SettingsImporter->Skeleton;
	AnimSequence->SetSkeleton(Skeleton);
	AnimSequence->GetController().OpenBracket(FText::FromString("Importing UEAnim Animation"));
	AnimSequence->GetController().InitializeModel();
	AnimSequence->ResetAnimation();

	AnimSequence->GetController().SetFrameRate(FFrameRate(Data.FramesPerSecond, 1));
	AnimSequence->GetController().SetNumberOfFrames(FFrameNumber(Data.NumFrames));

	FScopedSlowTask ImportTask(Data.Tracks.Num(), FText::FromString("Importing UEAnim Animation"));
	ImportTask.MakeDialog(false);

	for (auto i = 0; i < Data.Tracks.Num(); i++)
	{
		ImportTask.EnterProgressFrame();
		auto PreviewMesh = Skeleton->GetPreviewMesh();
		auto BoneName = PreviewMesh->GetRefSkeleton().GetBoneName(i);
		AnimSequence->GetController().AddBoneCurve(BoneName);

		auto PositionalKeys = Data.Tracks[i].TrackPosKeys;
		auto RotationalKeys = Data.Tracks[i].TrackRotKeys;
		auto ScaleKeys = Data.Tracks[i].TrackScaleKeys;
		for (auto j = 0; j < PositionalKeys.Num(); j++)
		{
			auto Value = PositionalKeys[i].VectorValue;
			FTransform Transform = FTransform(FVector(Value));
			AnimSequence->AddKeyToSequence(PositionalKeys[j].Frame / Data.NumFrames, BoneName, Transform);

		}
		for (auto k = 0; k < RotationalKeys.Num(); k++)
		{
			auto Value = RotationalKeys[k].QuatValue;
			FTransform Transform = FTransform(FQuat(Value));
			AnimSequence->AddKeyToSequence(PositionalKeys[k].Frame / Data.NumFrames, BoneName, Transform);
		}
		for (auto l = 0; l < ScaleKeys.Num(); l++)
		{
			auto Value = ScaleKeys[l].VectorValue;
			FTransform Transform = FTransform(FQuat::Identity, FVector::ZeroVector, FVector(Value));
			AnimSequence->AddKeyToSequence(PositionalKeys[l].Frame / Data.NumFrames, BoneName, Transform);
		}
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