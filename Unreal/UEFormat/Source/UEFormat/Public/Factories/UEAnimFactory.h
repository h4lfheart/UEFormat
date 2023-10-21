// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "Readers/UEAnimReader.h"
#include "Widgets/EAnimImportOptions.h"
#include "UEAnimFactory.generated.h"


UCLASS(hidecategories = Object)
class UEFORMAT_API UEAnimFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

		UPROPERTY()
		UEAnimImportOptions* SettingsImporter;

	bool bImport;
	bool bImportAll;

	virtual UObject* FactoryCreateFile(UClass* Class, UObject* Parent, FName Name, EObjectFlags Flags, const FString& Filename, const TCHAR* Params, FFeedbackContext* Warn, bool& bOutOperationCanceled) override;

	static void InterpolateVectorKeys(const TArray<FVectorKey>& Keys, TArray<FVector3f>& FinalKeys, int FrameIndex, float DataFramesPerSecond, FVector3f DefaultValue);
};