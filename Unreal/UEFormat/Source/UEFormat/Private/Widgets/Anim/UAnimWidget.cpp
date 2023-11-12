// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/Anim/UAnimWidget.h"
#include "SPrimaryButton.h"
#include "SlateOptMacros.h"
#include "Widgets/Anim/UAnimImportOptions.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void UAnimWidget::Construct(const FArguments& InArgs)
{
	WidgetWindow = InArgs._WidgetWindow;
	FPropertyEditorModule& EditModule = FModuleManager::Get().GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.bHideSelectionTip = true;
	TSharedRef<IDetailsView> Details = EditModule.CreateDetailView(DetailsViewArgs);
	EditModule.CreatePropertyTable();
	UObject* Container = NewObject<UAnimImportOptions>();
	Stun = Cast<UAnimImportOptions>(Container);
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
		    .OnClicked(this, &UAnimWidget::OnImport)
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2)
		[
			SNew(SButton)
			.Text(FText::FromString(TEXT("Apply to All")))
		.OnClicked(this, &UAnimWidget::OnImportAll)
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2)
		[
			SNew(SButton)
			.Text(FText::FromString(TEXT("Cancel")))
		.OnClicked(this, &UAnimWidget::OnCancel)
		]
		]
		]
	// Apply/Apply to All/Cancel
		];


}
bool UAnimWidget::ShouldImport()
{
	return UserDlgResponse == UAnimImportOptionDlgResponse::Import;
}
bool UAnimWidget::ShouldImportAll()
{
	return UserDlgResponse == UAnimImportOptionDlgResponse::ImportAll;
}
FReply UAnimWidget::OnImportAll()
{
	UserDlgResponse = UAnimImportOptionDlgResponse::ImportAll;
	return HandleImport();
}
FReply UAnimWidget::OnImport()
{
	UserDlgResponse = UAnimImportOptionDlgResponse::Import;
	return HandleImport();
}
FReply UAnimWidget::OnCancel()
{
	UserDlgResponse = UAnimImportOptionDlgResponse::Cancel;
	return FReply::Handled();
}
FReply UAnimWidget::HandleImport()
{
	if (WidgetWindow.IsValid())
	{
		WidgetWindow.Pin()->RequestDestroyWindow();
	}
	return FReply::Handled();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
