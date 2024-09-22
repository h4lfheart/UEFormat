// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/SkelMesh/UEFSkelMeshWidget.h"
#include "SPrimaryButton.h"
#include "SlateOptMacros.h"
#include "Widgets/SkelMesh/UEFSkelMeshImportOptions.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void UEFSkelMeshWidget::Construct(const FArguments& InArgs)
{
	WidgetWindow = InArgs._WidgetWindow;
	FPropertyEditorModule& EditModule = FModuleManager::Get().GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.bHideSelectionTip = true;
	TSharedRef<IDetailsView> Details = EditModule.CreateDetailView(DetailsViewArgs);
	EditModule.CreatePropertyTable();
	UObject* Container = NewObject<UEFSkelMeshImportOptions>();
	Stun = Cast<UEFSkelMeshImportOptions>(Container);
	Details->SetObject(Container);
	Details->SetEnabled(true);

	this->ChildSlot
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::Get().GetBrush(TEXT("Brushes.Panel")))
		.Padding(10)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBox)
			.Padding(FMargin(3))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		]
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(2)
		[
			SNew(SCheckBox)
			.OnCheckStateChanged(this, &UEFSkelMeshWidget::OnCreateMaterialsChanged)
			.IsChecked(this, &UEFSkelMeshWidget::IsCreateMaterialsChecked)
			.Content()
			[
				SNew(STextBlock).Text(FText::FromString(TEXT("Create Materials")))
			]
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(2)
		[
			SNew(SBox)
			.WidthOverride(400)
		[
			Details
		]
		]
	+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		    .HAlign(HAlign_Right)
		    .Padding(2)
		[
			SNew(SPrimaryButton)
			.Text(FText::FromString(TEXT("Apply")))
		    .OnClicked(this, &UEFSkelMeshWidget::OnImport)
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2)
		[
			SNew(SButton)
			.Text(FText::FromString(TEXT("Apply to All")))
		.OnClicked(this, &UEFSkelMeshWidget::OnImportAll)
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2)
		[
			SNew(SButton)
			.Text(FText::FromString(TEXT("Cancel")))
		.OnClicked(this, &UEFSkelMeshWidget::OnCancel)
		]
		]
		]
		];


}
bool UEFSkelMeshWidget::ShouldImport()
{
	return UserDlgResponse == UEFSkelMeshImportOptionDlgResponse::Import;
}
bool UEFSkelMeshWidget::ShouldImportAll()
{
	return UserDlgResponse == UEFSkelMeshImportOptionDlgResponse::ImportAll;
}
FReply UEFSkelMeshWidget::OnImportAll()
{
	UserDlgResponse = UEFSkelMeshImportOptionDlgResponse::ImportAll;
	return HandleImport();
}
FReply UEFSkelMeshWidget::OnImport()
{
	UserDlgResponse = UEFSkelMeshImportOptionDlgResponse::Import;
	return HandleImport();
}
FReply UEFSkelMeshWidget::OnCancel()
{
	UserDlgResponse = UEFSkelMeshImportOptionDlgResponse::Cancel;
	return FReply::Handled();
}
FReply UEFSkelMeshWidget::HandleImport()
{
	if (WidgetWindow.IsValid())
	{
		WidgetWindow.Pin()->RequestDestroyWindow();
	}
	return FReply::Handled();
}

// Add these methods to the UEFSkelMeshWidget class
void UEFSkelMeshWidget::OnCreateMaterialsChanged(ECheckBoxState NewState)
{
	if (WidgetWindow.IsValid())
	{
		Stun->bCreateMaterials = (NewState == ECheckBoxState::Checked);
	}
}

ECheckBoxState UEFSkelMeshWidget::IsCreateMaterialsChecked() const
{
	if (WidgetWindow.IsValid() && Stun->bCreateMaterials)
	{
		return ECheckBoxState::Checked;
	}
	return ECheckBoxState::Unchecked;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
