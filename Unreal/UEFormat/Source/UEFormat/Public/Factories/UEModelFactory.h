// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "../Public/Readers/UEModelReader.h"
#include "UEModelFactory.generated.h"

/**
 * Implements a factory for UnrealMODEL mesh objects.
 */
 
UCLASS(hidecategories=Object)
class UEFORMAT_API UEModelFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

	virtual UObject* FactoryCreateFile(UClass* Class, UObject* Parent, FName Name, EObjectFlags Flags, const FString& Filename, const TCHAR* Params, FFeedbackContext* Warn, bool& bOutOperationCanceled) override;

	UStaticMesh* CreateStaticMesh(UEModelReader& Data, UObject* Parent, EObjectFlags Flags);

	USkeletalMesh* CreateSkeletalMeshFromStatic(UEModelReader& Data, UStaticMesh* Mesh, EObjectFlags Flags);

	USkeleton* CreateSkeleton(UPackage* ParentPackage, EObjectFlags Flags, UEModelReader& Data, FReferenceSkeleton& RefSkeleton, FSkeletalMeshImportData& SkeletalMeshImportData);

	void ProcessSkeleton(const FSkeletalMeshImportData& ImportData, const USkeleton* Skeleton, FReferenceSkeleton& RefSkeleton);
};
