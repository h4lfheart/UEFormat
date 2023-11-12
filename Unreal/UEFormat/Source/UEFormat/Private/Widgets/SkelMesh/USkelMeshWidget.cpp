// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/SkelMesh/USkelMeshWidget.h"
#include "SPrimaryButton.h"
#include "SlateOptMacros.h"
#include "Widgets/SkelMesh/USkelMeshImportOptions.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void USkelMeshWidget::Construct(const FArguments& InArgs)
{
	WidgetWindow = InArgs._WidgetWindow;
	FPropertyEditorModule& EditModule = FModuleManager::Get().GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.bHideSelectionTip = true;
	TSharedRef<IDetailsView> Details = EditModule.CreateDetailView(DetailsViewArgs);
	EditModule.CreatePropertyTable();
	UObject* Container = NewObject<USkelMeshImportOptions>();
	Stun = Cast<USkelMeshImportOptions>(Container);
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

	// Data row struct
	// Curve interpolation
	// Details panel
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
		    .OnClicked(this, &USkelMeshWidget::OnImport)
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2)
		[
			SNew(SButton)
			.Text(FText::FromString(TEXT("Apply to All")))
		.OnClicked(this, &USkelMeshWidget::OnImportAll)
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2)
		[
			SNew(SButton)
			.Text(FText::FromString(TEXT("Cancel")))
		.OnClicked(this, &USkelMeshWidget::OnCancel)
		]
		]
		]
	// Apply/Apply to All/Cancel
		];


}
bool USkelMeshWidget::ShouldImport()
{
	return UserDlgResponse == USkelMeshImportOptionDlgResponse::Import;
}
bool USkelMeshWidget::ShouldImportAll()
{
	return UserDlgResponse == USkelMeshImportOptionDlgResponse::ImportAll;
}
FReply USkelMeshWidget::OnImportAll()
{
	UserDlgResponse = USkelMeshImportOptionDlgResponse::ImportAll;
	return HandleImport();
}
FReply USkelMeshWidget::OnImport()
{
	UserDlgResponse = USkelMeshImportOptionDlgResponse::Import;
	return HandleImport();
}
FReply USkelMeshWidget::OnCancel()
{
	UserDlgResponse = USkelMeshImportOptionDlgResponse::Cancel;
	return FReply::Handled();
}
FReply USkelMeshWidget::HandleImport()
{
	if (WidgetWindow.IsValid())
	{
		WidgetWindow.Pin()->RequestDestroyWindow();
	}
	return FReply::Handled();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
