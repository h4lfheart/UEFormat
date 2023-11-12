// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UAnimImportOptions.h"
#include "Widgets/SCompoundWidget.h"

enum class UAnimImportOptionDlgResponse : uint8
{
	Import,
	ImportAll,
	Cancel
};

class UEFORMAT_API UAnimWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(UAnimWidget) : _WidgetWindow()
	{}

	SLATE_ARGUMENT(TSharedPtr<SWindow>, WidgetWindow)
	SLATE_END_ARGS()

	UAnimWidget() : UserDlgResponse(UAnimImportOptionDlgResponse::Cancel)
	{}
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	/** A property view to edit advanced options */
	TSharedPtr< class IDetailsView > PropertyView;

	UPROPERTY(Category = MapsAndSets, EditAnywhere)
	mutable UAnimImportOptions* Stun;

	bool bShouldImportAll;
	/** If we should import */
	bool ShouldImport();

	/** If the current settings should be applied to all items being imported */
	bool ShouldImportAll();


	FReply OnImportAll();
	/** Called when 'Apply' button is pressed */
	FReply OnImport();

	FReply OnCancel();
private:
	UAnimImportOptionDlgResponse	UserDlgResponse;
	FReply HandleImport();

	/** Window that owns us */
	TWeakPtr< SWindow >	WidgetWindow;
};
