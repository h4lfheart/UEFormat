// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UAnimImportOptions.generated.h"

UCLASS(config = Engine, defaultconfig, transient)
class UEFORMAT_API UAnimImportOptions : public UObject
{
	GENERATED_BODY()
public:
	UAnimImportOptions();
	UPROPERTY( EditAnywhere, Category = "Import Settings")
	TObjectPtr<USkeleton> Skeleton;
	bool bInitialized;


	
};
