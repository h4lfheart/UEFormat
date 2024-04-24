// Fill out your copyright notice in the Description page of Project Settings.

#include "Factories/UEModelFactory.h"
#include "AssetToolsModule.h"
#include "StaticMeshAttributes.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/SkinnedAssetCommon.h"
#include "Rendering/SkeletalMeshLODImporterData.h"
#include "SkeletalMeshModelingToolsMeshConverter.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/StaticMesh.h"
#include "Widgets/SkelMesh/USkelMeshWidget.h"

/* UTextAssetFactory structors
 *****************************************************************************/
UEModelFactory::UEModelFactory( const FObjectInitializer& ObjectInitializer )
	: Super(ObjectInitializer)
{
	Formats.Add(TEXT("uemodel; UEMODEL Mesh File"));
	SupportedClass = UStaticMesh::StaticClass();
	bCreateNew = false;
	bEditorImport = true;
	SettingsImporter = CreateDefaultSubobject<USkelMeshImportOptions>(TEXT("Skeletal Mesh Options"));
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
		USkeleton* Skeleton = nullptr;
		USkeletalMesh* SkeletalMesh = CreateSkeletalMeshFromStatic(Data, Mesh, Skeleton, Flags);
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
	MeshDesc.SetNumUVChannels(Data.TextureCoordinates.Num());

	TArray<FVertexInstanceID> VertexInstanceIDs;
	const auto VertexPositions = Attributes.GetVertexPositions();
	const auto VertexInstanceNormals = Attributes.GetVertexInstanceNormals();
	const auto VertexInstanceTangents = Attributes.GetVertexInstanceTangents();
	const auto VertexInstanceBinormalSigns = Attributes.GetVertexInstanceBinormalSigns();
	const auto VertexInstanceColors = Attributes.GetVertexInstanceColors();
	const auto VertexInstanceUVs = Attributes.GetVertexInstanceUVs();
	VertexInstanceUVs.SetNumChannels(Data.TextureCoordinates.Num());

	for (auto i = 0; i < Data.Vertices.Num(); i++)
	{
		FVertexID VertexID = MeshDesc.CreateVertex();
		FVertexInstanceID VertexInstanceID = MeshDesc.CreateVertexInstance(VertexID);
		VertexInstanceIDs.Add(VertexInstanceID);

		VertexPositions.Set(VertexID, FVector3f(Data.Vertices[i].X, -Data.Vertices[i].Y, Data.Vertices[i].Z));
		if (Data.Normals.Num() > 0) {
			VertexInstanceBinormalSigns.Set(VertexInstanceID, Data.Normals[i].X);
			VertexInstanceNormals.Set(VertexInstanceID, FVector3f(Data.Normals[i].Y, -Data.Normals[i].Z, Data.Normals[i].W));
		}
		if (Data.Tangents.Num() > 0) {
			VertexInstanceTangents.Set(VertexInstanceID, FVector3f(Data.Tangents[i].X, -Data.Tangents[i].Y, Data.Tangents[i].Z));
		}
		if (Data.VertexColors.Num() > 0) {
			VertexInstanceColors.Set(VertexInstanceID, FVector4f(Data.VertexColors[0].Data[i]));
		}
		for (auto u = 0; u < Data.TextureCoordinates.Num(); u++)
		{
			VertexInstanceUVs.Set(VertexInstanceID, u, Data.TextureCoordinates[u][i]);
		}
	}

	for (auto m = 0; m < Data.Materials.Num(); m++)
	{
		const FName MatName = Data.Materials[m].Name.c_str();
		const auto FirstIndex = Data.Materials[m].FirstIndex;
		const auto NumFaces = Data.Materials[m].NumFaces;

		FPolygonGroupID PolygonGroup = MeshDesc.CreatePolygonGroup();
		for (auto i = FirstIndex; i < FirstIndex + (NumFaces * 3); i += 3) //Winding order
		{
			FVertexInstanceID& VI0 = VertexInstanceIDs[Data.Indices[i]];
			FVertexInstanceID& VI1 = VertexInstanceIDs[Data.Indices[i + 1]];
			FVertexInstanceID& VI2 = VertexInstanceIDs[Data.Indices[i + 2]];
			MeshDesc.CreatePolygon(PolygonGroup, { VI0, VI1, VI2 });
		}
		Attributes.GetPolygonGroupMaterialSlotNames()[PolygonGroup] = MatName;

		FStaticMaterial&& StaticMat = FStaticMaterial();
		StaticMat.MaterialSlotName = MatName;
		StaticMat.ImportedMaterialSlotName = MatName;
		StaticMesh->GetStaticMaterials().Add(StaticMat);
	}

	UStaticMesh::FBuildMeshDescriptionsParams BuildParams;
	StaticMesh->NaniteSettings.bEnabled = false;

	StaticMesh->BuildFromMeshDescriptions({ &MeshDesc }, BuildParams);
	StaticMesh->PostEditChange();
	FAssetRegistryModule::AssetCreated(StaticMesh);
	return StaticMesh;
}

USkeletalMesh* UEModelFactory::CreateSkeletalMeshFromStatic(UEModelReader& Data, UStaticMesh* Mesh, USkeleton* Skeleton, EObjectFlags Flags)
{
	FReferenceSkeleton RefSkeleton;
	FSkeletalMeshImportData SkelMeshImportData;

	Skeleton = CreateSkeleton(Mesh->GetPackage(), Flags, Data, RefSkeleton, SkelMeshImportData);

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
		const auto Weight = Data.Weights[i];
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
	FAssetRegistryModule::AssetCreated(SkeletalMesh);

	Skeleton->MergeAllBonesToBoneTree(SkeletalMesh);
	Skeleton->SetPreviewMesh(SkeletalMesh);
	Skeleton->PostEditChange();
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
		auto Location = FVector3f(Data.Bones[i].BonePos.X, -Data.Bones[i].BonePos.Y, Data.Bones[i].BonePos.Z);
		Transform.SetLocation(Location);
		auto Rotation = FQuat4f(Data.Bones[i].BoneRot.X, -Data.Bones[i].BoneRot.Y, Data.Bones[i].BoneRot.Z, -Data.Bones[i].BoneRot.W);
		Transform.SetRotation(Rotation);

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