// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/Factories/UEModelFactory.h"
#include "AssetToolsModule.h"
#include "SkeletalMeshAttributes.h"
#include "StaticMeshAttributes.h"
#include "../Public/Readers/UEModelReader.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/SkinnedAssetCommon.h"
#include "Rendering/SkeletalMeshLODImporterData.h"
#include "SkeletalMeshModelingToolsMeshConverter.h"
#include "AssetRegistry/AssetRegistryModule.h"


/* UTextAssetFactory structors
 *****************************************************************************/
UEModelFactory::UEModelFactory( const FObjectInitializer& ObjectInitializer )
	: Super(ObjectInitializer)
{
	Formats.Add(TEXT("uemodel;UEMODEL Mesh File"));
	SupportedClass = UStaticMesh::StaticClass();
	bCreateNew = false;
	bEditorImport = true;
}

/* UFactory overrides
 *****************************************************************************/
UObject* UEModelFactory::FactoryCreateFile(UClass* Class, UObject* Parent, FName Name, EObjectFlags Flags, const FString& Filename, const TCHAR* Params, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{
	UEModelReader Data = UEModelReader(Filename);
	if (!Data.Read()) return nullptr;

	UStaticMesh* Mesh = CreateStaticMesh(Data, Parent, Flags);

	if (Data.Bones.Num())
	{
		USkeletalMesh* SkeletalMesh = CreateSkeletalMeshFromStatic(Data, Mesh, Flags);
		Mesh->RemoveFromRoot();
		Mesh->MarkAsGarbage();
		return SkeletalMesh;
	}
	return Mesh;
}

UStaticMesh* UEModelFactory::CreateStaticMesh(UEModelReader& Data, UObject* Parent, EObjectFlags Flags) {
	FName ObjectName = Data.Header.ObjectName.c_str();
	if (Data.Bones.Num()){ ObjectName = "TEMP"; }

	UStaticMesh* StaticMesh = NewObject<UStaticMesh>(Parent, ObjectName, Flags);

	FMeshDescription MeshDesc;
	FStaticMeshAttributes Attributes(MeshDesc);
	Attributes.Register();

	//Reserve space
	MeshDesc.ReserveNewVertices(Data.Vertices.Num());
	MeshDesc.ReserveNewVertexInstances(Data.Vertices.Num());
	MeshDesc.ReserveNewPolygons(Data.Indices.Num() / 3);
	MeshDesc.ReserveNewPolygonGroups(Data.Materials.Num());

	TArray<FVertexInstanceID> VertexInstanceIDs;
	const auto VertexPositions = Attributes.GetVertexPositions();
	const auto VertexInstanceNormals = Attributes.GetVertexInstanceNormals();
	const auto VertexInstanceTangents = Attributes.GetVertexInstanceTangents();
	const auto VertexInstanceUVs = Attributes.GetVertexInstanceUVs();
	const auto VertexInstanceColors = Attributes.GetVertexInstanceColors();

	VertexInstanceUVs.SetNumChannels(Data.TextureCoordinates.Num());

	for (auto i = 0; i < Data.Vertices.Num(); i++)
	{
		FVertexID VertexID = MeshDesc.CreateVertex();
		FVertexInstanceID VertexInstanceID = MeshDesc.CreateVertexInstance(VertexID);
		VertexInstanceIDs.Add(VertexInstanceID);

		VertexPositions[VertexID] = Data.Vertices[i];
		if (Data.Normals.Num() > 0) {
			VertexInstanceNormals[VertexInstanceID] = Data.Normals[i];
		}
		if (Data.Tangents.Num() > 0) {
			VertexInstanceTangents[VertexInstanceID] = Data.Tangents[i];
		}
		if (Data.VertexColors.Num() > 0) {
			VertexInstanceColors[VertexInstanceID] = FVector4f(Data.VertexColors[i]);
		}
		for (auto u = 0; u < Data.TextureCoordinates.Num(); u++)
		{
			VertexInstanceUVs.Set(VertexInstanceID, u, Data.TextureCoordinates[u][i]);
		}
	}

	for (auto m = 0; m < Data.Materials.Num(); m++)
	{
		FName MatName = Data.Materials[m].Name.c_str();
		auto FirstIndex = Data.Materials[m].FirstIndex;
		auto NumFaces = Data.Materials[m].NumFaces;

		FPolygonGroupID PolygonGroup = MeshDesc.CreatePolygonGroup();
		for (auto i = FirstIndex; i < FirstIndex + (NumFaces * 3); i += 3) //Clockwise winding order
		{
			FVertexInstanceID& VI0 = VertexInstanceIDs[Data.Indices[i]];
			FVertexInstanceID& VI1 = VertexInstanceIDs[Data.Indices[i + 1]];
			FVertexInstanceID& VI2 = VertexInstanceIDs[Data.Indices[i + 2]];
			MeshDesc.CreatePolygon(PolygonGroup, { VI0, VI2, VI1 });
		}
		Attributes.GetPolygonGroupMaterialSlotNames()[PolygonGroup] = MatName;

		FStaticMaterial&& StaticMat = FStaticMaterial();
		StaticMat.MaterialSlotName = MatName;
		StaticMat.ImportedMaterialSlotName = MatName;
		StaticMesh->GetStaticMaterials().Add(StaticMat);
	}

	UStaticMesh::FBuildMeshDescriptionsParams BuildParams;
	BuildParams.bBuildSimpleCollision = false;
	StaticMesh->NaniteSettings.bEnabled = false;

	StaticMesh->BuildFromMeshDescriptions({ &MeshDesc }, BuildParams);
	return StaticMesh;
}

USkeletalMesh* UEModelFactory::CreateSkeletalMeshFromStatic(UEModelReader& Data, UStaticMesh* Mesh, EObjectFlags Flags)
{
	FReferenceSkeleton RefSkeleton;
	FSkeletalMeshImportData SkelMeshImportData;
	USkeleton* Skeleton = CreateSkeleton(Mesh->GetPackage(), Flags, Data, RefSkeleton, SkelMeshImportData);

	USkeletalMeshFromStaticMeshFactory* SkeletalMeshFactory = NewObject<USkeletalMeshFromStaticMeshFactory>();
	SkeletalMeshFactory->StaticMesh = Mesh;
	SkeletalMeshFactory->Skeleton = Skeleton;
	SkeletalMeshFactory->ReferenceSkeleton = RefSkeleton;

	IAssetTools& AssetTools = FAssetToolsModule::GetModule().Get();
	USkeletalMesh* SkeletalMesh = Cast<USkeletalMesh>(AssetTools.CreateAsset(Data.Header.ObjectName.c_str(), FPackageName::GetLongPackagePath(Mesh->GetPackage()->GetName()), USkeletalMesh::StaticClass(), SkeletalMeshFactory));

	SkeletalMesh->LoadLODImportedData(0, SkelMeshImportData);
	TArray<SkeletalMeshImportData::FRawBoneInfluence> Influences;
	for (auto i = 0; i < Data.Weights.Num(); i++)
	{
		auto Weight = Data.Weights[i];
		SkeletalMeshImportData::FRawBoneInfluence Influence;
		Influence.BoneIndex = int32(Weight.WeightBoneIndex);
		Influence.VertexIndex = Weight.WeightVertexIndex;
		Influence.Weight = Weight.WeightAmount;
		Influences.Add(Influence);
	}
	SkelMeshImportData.Influences = Influences;
	SkeletalMesh->SaveLODImportedData(0, SkelMeshImportData);

	SkeletalMesh->CalculateInvRefMatrices();
	const FSkeletalMeshBuildSettings BuildOptions;
	SkeletalMesh->GetLODInfo(0)->BuildSettings = BuildOptions;
	SkeletalMesh->SetImportedBounds(FBoxSphereBounds(FBoxSphereBounds3f(FBox3f(SkelMeshImportData.Points))));

	SkeletalMesh->SetSkeleton(Skeleton);
	SkeletalMesh->PostEditChange();

	Skeleton->MergeAllBonesToBoneTree(SkeletalMesh);
	Skeleton->SetPreviewMesh(SkeletalMesh);
	Skeleton->PostEditChange();

	FAssetRegistryModule::AssetCreated(SkeletalMesh);
	FAssetRegistryModule::AssetCreated(Skeleton);
	return SkeletalMesh;
}

USkeleton* UEModelFactory::CreateSkeleton(UPackage* ParentPackage, EObjectFlags Flags, UEModelReader& Data, FReferenceSkeleton& RefSkeleton, FSkeletalMeshImportData& SkeletalMeshImportData)
{
	FString SkeletonName = (Data.Header.ObjectName + "_Skeleton").c_str();
	auto SkeletonPackage = CreatePackage(*FPaths::Combine(FPaths::GetPath(ParentPackage->GetPathName()), SkeletonName));
	USkeleton* Skeleton = NewObject<USkeleton>(SkeletonPackage, FName(*SkeletonName), Flags);

	FReferenceSkeletonModifier RefSkeletonModifier(RefSkeleton, Skeleton);

	TArray<FString> AddedBoneNames;
	for (auto i = 0; i < Data.Bones.Num(); i++)
	{
		SkeletalMeshImportData::FBone Bone;
		Bone.Name = Data.Bones[i].BoneName.c_str();

		if (AddedBoneNames.Contains(Bone.Name)) continue;
		AddedBoneNames.Add(Bone.Name);
		Bone.ParentIndex = (i > 0) ? Data.Bones[i].BoneParentIndex : INDEX_NONE;

		FTransform3f Transform;
		Transform.SetLocation(Data.Bones[i].BonePos);
		Transform.SetRotation(Data.Bones[i].BoneRot);

		SkeletalMeshImportData::FJointPos BonePos;
		BonePos.Transform = Transform;
		BonePos.Length = 1;
		BonePos.XSize = 1;
		BonePos.YSize = 1;
		BonePos.ZSize = 1;
		Bone.BonePos = BonePos;
		SkeletalMeshImportData.RefBonesBinary.Add(Bone);

		const FMeshBoneInfo BoneInfo(FName(*Bone.Name), Bone.Name, Bone.ParentIndex);
		RefSkeletonModifier.Add(BoneInfo, FTransform(Bone.BonePos.Transform));
	}
	return Skeleton;
}