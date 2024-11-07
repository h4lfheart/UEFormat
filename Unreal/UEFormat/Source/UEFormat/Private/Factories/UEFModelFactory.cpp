#include "Factories/UEFModelFactory.h"

#include "SkeletalMeshAttributes.h"
#include "StaticMeshAttributes.h"

#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Rendering/SkeletalMeshLODImporterData.h"

UEFModelFactory::UEFModelFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Formats.Add(TEXT("uemodel; UEModel Mesh File"));
	SupportedClass = UObject::StaticClass();
	bCreateNew = false;
	bEditorImport = true;
}

UObject* UEFModelFactory::FactoryCreateFile(UClass* Class, UObject* Parent, FName Name, EObjectFlags Flags, const FString& Filename, const TCHAR* Params, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{
	UEFModelReader Data = UEFModelReader(Filename);

	//empty mesh
	if (!Data.Read() || Data.LODs.Num() == 0)
		return nullptr;

	//skeletal mesh
	if(Data.Skeleton.Bones.Num() > 0)
		return CreateSkeletalMesh(Data, Name, Parent, Flags);

	//static mesh
	return CreateStaticMesh(Data, Name, Parent, Flags);
}

UStaticMesh* UEFModelFactory::CreateStaticMesh(UEFModelReader& Data, FName Name, UObject* Parent, EObjectFlags Flags)
{
	UStaticMesh* StaticMesh = NewObject<UStaticMesh>(Parent, Name, Flags);

	for(int i = 0; i < Data.LODs.Num(); i++)
	{
		StaticMesh->AddSourceModel(); //important!

		FMeshDescription* MeshDescription = StaticMesh->CreateMeshDescription(i);
		FillStaticMeshMeshAttributes(*MeshDescription, Data.LODs[i]);
		StaticMesh->CommitMeshDescription(i);
		StaticMesh->GetSourceModel(i).BuildSettings.bGenerateLightmapUVs = false;
	}

	for (auto [MatIndex, MatName, FirstIndex, NumFaces] : Data.LODs[0].Materials) {
		FStaticMaterial StaticMat = FStaticMaterial();
		StaticMat.MaterialSlotName = MatName.c_str();
		StaticMat.ImportedMaterialSlotName = MatName.c_str();
		StaticMesh->GetStaticMaterials().Add(StaticMat);
	}

	StaticMesh->SetLightMapCoordinateIndex(Data.LODs[0].TextureCoordinates.Num() -1);
	StaticMesh->PostEditChange();
	FAssetRegistryModule::AssetCreated(StaticMesh);
	return StaticMesh;
}

void UEFModelFactory::FillStaticMeshAttributes(FMeshDescription& MeshDesc, FLODData& Data)
{
    FStaticMeshAttributes Attributes(MeshDesc);
    Attributes.Register();

    const int NumVertices = Data.Vertices.Num();
	const int NumUVChannels = Data.TextureCoordinates.Num();
	const int NumTriangles = Data.Indices.Num() / 3;
	const int NumPolyGroups = Data.Materials.Num();

	MeshDesc.ReserveNewVertices(NumVertices);
	MeshDesc.ReserveNewVertexInstances(NumVertices);
	MeshDesc.ReserveNewTriangles(NumTriangles);
	MeshDesc.ReserveNewPolygonGroups(NumPolyGroups);

	MeshDesc.SetNumUVChannels(NumUVChannels);
	MeshDesc.VertexInstanceAttributes().SetAttributeChannelCount(MeshAttribute::VertexInstance::TextureCoordinate, NumUVChannels);
	for (int UVChannelIndex = 0; UVChannelIndex < NumUVChannels; ++UVChannelIndex)
		MeshDesc.ReserveNewUVs(NumVertices, UVChannelIndex);

	TVertexAttributesRef<FVector3f> VertexPositions = Attributes.GetVertexPositions();
	TVertexInstanceAttributesRef<FVector3f> VertexInstanceNormals = Attributes.GetVertexInstanceNormals();
	TVertexInstanceAttributesRef<FVector3f> VertexInstanceTangents = Attributes.GetVertexInstanceTangents();
	TVertexInstanceAttributesRef<float> VertexInstanceBinormalSigns = Attributes.GetVertexInstanceBinormalSigns();
	TVertexInstanceAttributesRef<FVector4f> VertexInstanceColors = Attributes.GetVertexInstanceColors();
	TVertexInstanceAttributesRef<FVector2f> VertexInstanceUVs = Attributes.GetVertexInstanceUVs();
	TPolygonGroupAttributesRef<FName> PolygonGroupMaterialSlotNames = Attributes.GetPolygonGroupMaterialSlotNames();

	for (int InVertIndex = 0; InVertIndex < NumVertices; ++InVertIndex)
	{
		const FVertexID VertexID(InVertIndex);
		const FVertexInstanceID VertexInstanceID(InVertIndex);

		//TODO: deduplicate vertex positions?
		MeshDesc.CreateVertexWithID(VertexID);
		MeshDesc.CreateVertexInstanceWithID(VertexInstanceID, VertexID);

		//invert y
		const FVector3f Position = FVector3f(Data.Vertices[InVertIndex].X, -Data.Vertices[InVertIndex].Y, Data.Vertices[InVertIndex].Z);
		const FVector3f Tangent = FVector3f(Data.Tangents[InVertIndex].X, -Data.Tangents[InVertIndex].Y, Data.Tangents[InVertIndex].Z);
		const FVector3f Normal = FVector3f(Data.Normals[InVertIndex].Y, -Data.Normals[InVertIndex].Z, Data.Normals[InVertIndex].W);
		const float BinormalSign = Data.Normals[InVertIndex].X;
		const FColor Color = Data.VertexColors.IsValidIndex(0) && Data.VertexColors[0].Data.IsValidIndex(InVertIndex) ? Data.VertexColors[0].Data[InVertIndex] : FColor::White; //first vertex color layer

		VertexPositions.Set(VertexID, Position);
		VertexInstanceNormals.Set(VertexInstanceID, Normal);
		VertexInstanceTangents.Set(VertexInstanceID, Tangent);
		VertexInstanceBinormalSigns.Set(VertexInstanceID, BinormalSign);
		VertexInstanceColors.Set(VertexInstanceID, FVector4f(FLinearColor(Color)));

		for (int UVChannelIndex = 0; UVChannelIndex < NumUVChannels; ++UVChannelIndex)
			VertexInstanceUVs.Set(VertexInstanceID, UVChannelIndex, Data.TextureCoordinates[UVChannelIndex][InVertIndex]);
	}

	for (auto [MatIndex, Name, FirstIndex, NumFaces] : Data.Materials)
	{
		const FPolygonGroupID PolygonGroupID = MeshDesc.CreatePolygonGroup();
		PolygonGroupMaterialSlotNames.Set(PolygonGroupID, Name.c_str());

		for (int TriIndex = 0; TriIndex < NumFaces; ++TriIndex)
		{
			const FVertexInstanceID TriVertInstanceIDs[] = {
				FVertexInstanceID(Data.Indices[FirstIndex + TriIndex * 3 + 0]),
				FVertexInstanceID(Data.Indices[FirstIndex + TriIndex * 3 + 1]),
				FVertexInstanceID(Data.Indices[FirstIndex + TriIndex * 3 + 2])
			};
			//MeshDesc.CreateTriangle(PolygonGroupID, MakeConstArrayView(TriVertInstanceIDs, 3));
		}
	}
}

USkeletalMesh* UEFModelFactory::CreateSkeletalMesh(UEFModelReader& Data, FName Name, UObject* Parent, EObjectFlags Flags)
{
	USkeletalMesh* SkeletalMesh = NewObject<USkeletalMesh>(Parent, Name, Flags);

	for(int i = 0; i < Data.LODs.Num(); i++)
	{
		FSkeletalMeshLODInfo LODInfo;
		SkeletalMesh->AddLODInfo(LODInfo); //important!

		FMeshDescription* MeshDescription = SkeletalMesh->CreateMeshDescription(i);
		FillSkeletalMeshAttributes(*MeshDescription, Data, i);
		SkeletalMesh->CommitMeshDescription(i);
	}

	for (auto [MatIndex, MatName, FirstIndex, NumFaces] : Data.LODs[0].Materials) {
		FSkeletalMaterial SkeletalMat = FSkeletalMaterial();
		SkeletalMat.MaterialSlotName = MatName.c_str();
		SkeletalMat.ImportedMaterialSlotName = MatName.c_str();
		SkeletalMesh->GetMaterials().Add(SkeletalMat);
	}

	//create and assign skeleton
	FReferenceSkeleton RefSkeleton;
	USkeleton* Skeleton = CreateSkeleton(Name.ToString(), Parent->GetPackage(), Flags, Data.Skeleton, RefSkeleton);
	Skeleton->MergeAllBonesToBoneTree(SkeletalMesh);
	Skeleton->SetPreviewMesh(SkeletalMesh);
	Skeleton->PostEditChange();
	FAssetRegistryModule::AssetCreated(Skeleton);

	SkeletalMesh->SetRefSkeleton(RefSkeleton);
	SkeletalMesh->SetSkeleton(Skeleton);

	SkeletalMesh->SetImportedBounds(FBoxSphereBounds(FBoxSphereBounds3f(FBox3f(Data.LODs[0].Vertices))));
	SkeletalMesh->PostEditChange();
	FAssetRegistryModule::AssetCreated(SkeletalMesh);
	return SkeletalMesh;
}

void UEFModelFactory::FillSkeletalMeshAttributes(FMeshDescription& MeshDesc, UEFModelReader& Data, int LODIndex)
{
	/*
	FillStaticMeshAttributes(MeshDesc, Data.LODs[LODIndex]);
	FSkeletalMeshAttributes SkeletalMeshAttributes(MeshDesc);
	SkeletalMeshAttributes.Register();

	//bones
	TMeshAttributesRef<FBoneID, FName> BoneNames = SkeletalMeshAttributes.GetBoneNames();
	TMeshAttributesRef<FBoneID, int> BoneParentIndices = SkeletalMeshAttributes.GetBoneParentIndices();
	TMeshAttributesRef<FBoneID, FTransform> BonePoses = SkeletalMeshAttributes.GetBonePoses();

	for (auto [BoneName, BoneParentIndex, BonePos, BoneRot] :  Data.Skeleton.Bones)
	{
		const FBoneID BoneID = SkeletalMeshAttributes.CreateBone();

		BoneNames.Set(BoneID, BoneName.c_str());
		BoneParentIndices.Set(BoneID, BoneParentIndex);

		FTransform Transform;
		//invert y
		Transform.SetLocation(FVector(BonePos.X, -BonePos.Y, BonePos.Z));
		Transform.SetRotation(FQuat(BoneRot.X, -BoneRot.Y, BoneRot.Z, -BoneRot.W));
		BonePoses.Set(BoneID, Transform);
	}

	//weights
	FSkinWeightsVertexAttributesRef VertexSkinWeights = SkeletalMeshAttributes.GetVertexSkinWeights();
	for (auto [WeightVertexIndex, WeightAmount, WeightBoneIndex] : Data.LODs[LODIndex].Weights)
	{
		auto VertexWeights = VertexSkinWeights.Get(WeightVertexIndex);
		UE::AnimationCore::FBoneWeight BoneWeight;
		BoneWeight.SetWeight(WeightAmount);
		BoneWeight.SetBoneIndex(WeightBoneIndex);
		UE::AnimationCore::FBoneWeightsSettings InSetting;
		VertexWeights.AddBoneWeight(BoneWeight, InSetting);
	}

	//morph targets
	for (auto [MorphName, MorphDeltas] :  Data.LODs[LODIndex].Morphs)
	{
		FName MorphFName = MorphName.c_str();
		SkeletalMeshAttributes.RegisterMorphTargetAttribute(MorphFName, true);
		auto VertexMorphPositionDelta = SkeletalMeshAttributes.GetVertexMorphPositionDelta(MorphFName);
		auto VertexInstanceMorphNormalDelta = SkeletalMeshAttributes.GetVertexInstanceMorphNormalDelta(MorphFName);
		for (auto [MorphPosition, MorphNormals, MorphVertexIndex] :  MorphDeltas)
		{
			//invert y
			const FVector3f CorrectedMorphPosition = FVector3f(MorphPosition.X, -MorphPosition.Y, MorphPosition.Z);
			const FVector3f CorrectedMorphNormal = FVector3f(MorphNormals.X, -MorphNormals.Y, MorphNormals.Z);
			VertexMorphPositionDelta.Set(MorphVertexIndex, CorrectedMorphPosition);
			VertexInstanceMorphNormalDelta.Set(MorphVertexIndex, CorrectedMorphNormal);
		}
	}
	*/
}

USkeleton* UEFModelFactory::CreateSkeleton(FString Name, UPackage* ParentPackage, EObjectFlags Flags, FSkeletonData& Data, FReferenceSkeleton& RefSkeleton)
{
	FString SkeletonName = Name + "_Skeleton";
	UPackage* SkeletonPackage = CreatePackage(*FPaths::Combine(FPaths::GetPath(ParentPackage->GetPathName()), SkeletonName));
	USkeleton* Skeleton = NewObject<USkeleton>(SkeletonPackage, FName(*SkeletonName), Flags);

	/*
	FReferenceSkeletonModifier RefSkeletonModifier(RefSkeleton, Skeleton);

	for (int i = 0; i < Data.Bones.Num(); i++)
	{
		FTransform Transform;
		auto Location = FVector(Data.Bones[i].BonePos.X, -Data.Bones[i].BonePos.Y, Data.Bones[i].BonePos.Z);
		Transform.SetLocation(Location);
		auto Rotation = FQuat(Data.Bones[i].BoneRot.X, -Data.Bones[i].BoneRot.Y, Data.Bones[i].BoneRot.Z, -Data.Bones[i].BoneRot.W);
		Transform.SetRotation(Rotation);
		FString BoneName = Data.Bones[i].BoneName.c_str();

		const FMeshBoneInfo BoneInfo(*BoneName, BoneName, (i > 0) ? Data.Bones[i].BoneParentIndex : INDEX_NONE);
		RefSkeletonModifier.Add(BoneInfo, Transform);
	}
	*/
	return Skeleton;
}