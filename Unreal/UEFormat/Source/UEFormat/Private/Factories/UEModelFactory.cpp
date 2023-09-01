// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/Factories/UEModelFactory.h"
#include "../Public/Readers/UEModelReader.h"
#include "RawMesh.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Editor/UnrealEd/Classes/Factories/MaterialInstanceConstantFactoryNew.h"
#include "EditorAssetLibrary.h"
#include "AssetToolsModule.h"
#include "ComponentReregisterContext.h"

/* UTextAssetFactory structors
 *****************************************************************************/

UEModelFactory::UEModelFactory( const FObjectInitializer& ObjectInitializer )
	: Super(ObjectInitializer)
{
	Formats.Add(TEXT("umodel;UMODEL Static Mesh File"));
	SupportedClass = UStaticMesh::StaticClass();
	bCreateNew = false;
	bEditorImport = true;
}

/* UFactory overrides
 *****************************************************************************/

UObject* UEModelFactory::FactoryCreateFile(UClass* Class, UObject* Parent, FName Name, EObjectFlags Flags, const FString& Filename, const TCHAR* Params, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{
	auto Data = UEModelReader(Filename);
	if (!Data.Read()) return nullptr;
	
	auto RawMesh = FRawMesh();

	for (auto Vertex : Data.Vertices)
	{
		RawMesh.VertexPositions.Add(Vertex);
	}
	
	for (auto Index : Data.Indices)
	{
		RawMesh.WedgeIndices.Add(Index);
	}
	
	for (auto Normal : Data.Normals)
	{
		RawMesh.WedgeTangentZ.Add(Normal);
		RawMesh.WedgeTangentX.Add(FVector3f::ZeroVector);
		RawMesh.WedgeTangentY.Add(FVector3f::ZeroVector);
	}
	
	RawMesh.WedgeTexCoords->SetNum(Data.TextureCoordinates.Num());
	for (auto i = 0; i < Data.TextureCoordinates.Num(); i++)
	{
		for (auto UV : Data.TextureCoordinates[i])
		{
			RawMesh.WedgeTexCoords[i].Add(FVector2f(UV.U, UV.V));
		}
	}
	
	if (Data.bHasVertexColors) {
		for (auto Color : Data.VertexColors)
		{
			RawMesh.WedgeColors.Add(Color);
		}
	}
	
	const auto StaticMesh = CastChecked<UStaticMesh>(CreateOrOverwriteAsset(UStaticMesh::StaticClass(), Parent, Name, Flags));


	RawMesh.FaceMaterialIndices.SetNum(Data.Indices.Num()/3);
	for (auto materialChunk : Data.Materials)
	{
		for (auto i = materialChunk.FirstIndex; i < materialChunk.FirstIndex + materialChunk.NumFaces; i++)
		{
			RawMesh.FaceMaterialIndices[i] = materialChunk.MatIndex;
			RawMesh.FaceSmoothingMasks.Add(1);
		}
	}
	
	for (auto i = 0; i < Data.Materials.Num(); i++)
	{
		auto MeshMaterial = Data.Materials[i];
		const auto MaterialAdd = CastChecked<UMaterial>(CreateOrOverwriteAsset(UMaterial::StaticClass(), Parent, MeshMaterial.MaterialName.data(), Flags));
		StaticMesh->GetStaticMaterials().Add(FStaticMaterial(MaterialAdd));
		StaticMesh->GetSectionInfoMap().Set(0, MeshMaterial.MatIndex, FMeshSectionInfo(i));
	}
	
	auto& SourceModel = StaticMesh->AddSourceModel();
	SourceModel.BuildSettings.bBuildReversedIndexBuffer = false;
	SourceModel.BuildSettings.bRecomputeTangents = false;
	SourceModel.BuildSettings.bGenerateLightmapUVs = false;
	SourceModel.BuildSettings.bComputeWeightedNormals = false;
	SourceModel.BuildSettings.bRecomputeNormals = !Data.bHasVertexNormals;

	StaticMesh->Build();
	StaticMesh->PostEditChange();
	FAssetRegistryModule::AssetCreated(StaticMesh);
	StaticMesh->MarkPackageDirty();

	FGlobalComponentReregisterContext RecreateComponents;
	
	return StaticMesh;
}


