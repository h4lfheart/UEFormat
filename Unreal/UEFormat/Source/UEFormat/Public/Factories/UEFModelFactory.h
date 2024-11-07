// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "SkeletalMeshAttributes.h"
#include "StaticMeshAttributes.h"

#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"

#include "Factories/Factory.h"
#include "Readers/UEFModelReader.h"

#include "UEFModelFactory.generated.h"

UCLASS(hidecategories=Object)
class UEFORMAT_API UEFModelFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

	virtual UObject* FactoryCreateFile(UClass* Class, UObject* Parent, FName Name, EObjectFlags Flags, const FString& Filename, const TCHAR* Params, FFeedbackContext* Warn, bool& bOutOperationCanceled) override;

	void FillStaticMeshMeshAttributes(FMeshDescription& MeshDesc, FLODData& Data);

	void FillSkeletalMeshAttributes(FMeshDescription& MeshDesc, UEFModelReader& Data, int LODIndex);

	UStaticMesh* CreateStaticMesh(UEFModelReader& Data, FName Name, UObject* Parent, EObjectFlags Flags);
	void FillStaticMeshAttributes(FMeshDescription& MeshDesc, FLODData& Data);

	USkeletalMesh* CreateSkeletalMesh(UEFModelReader& Data, FName Name, UObject* Parent, EObjectFlags Flags);

	USkeleton* CreateSkeleton(FString Name, UPackage* ParentPackage, EObjectFlags Flags, FSkeletonData& Data, FReferenceSkeleton& RefSkeleton);
};
