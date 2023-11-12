// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "USkelMeshImportOptions.generated.h"

UCLASS(config = Engine, defaultconfig, transient)
class UEFORMAT_API USkelMeshImportOptions : public UObject
{
	GENERATED_BODY()
public:
	USkelMeshImportOptions();
	UPROPERTY( EditAnywhere, Category = "Import Settings")
	TObjectPtr<USkeleton> Skeleton;
	bool bInitialized;


	
};
