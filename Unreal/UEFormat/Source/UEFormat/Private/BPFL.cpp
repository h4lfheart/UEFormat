#include "BPFL.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/Actor.h"
#include "Readers/UEFModelReader.h"
#include "Rendering/PositionVertexBuffer.h"
#include "StaticMeshComponentLODInfo.h"
#include "StaticMeshResources.h"
#include "VectorTypes.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "UObject/ConstructorHelpers.h"

void UBPFL::SetMaterial(UObject* Mesh, int MaterialIndex, UMaterialInterface* Material)
{
	if(UStaticMesh* StaticMesh = Cast<UStaticMesh>(Mesh))
	{
		int RealMaterialIndex = FMath::Clamp(MaterialIndex, 0, StaticMesh->GetStaticMaterials().Num() - 1);
		StaticMesh->SetMaterial(RealMaterialIndex, Material);
		StaticMesh->PostEditChange();
	}
	else if(USkeletalMesh* SkeletalMesh = Cast<USkeletalMesh>(Mesh))
	{
		int RealMaterialIndex = FMath::Clamp(MaterialIndex, 0, SkeletalMesh->GetMaterials().Num() - 1);
		SkeletalMesh->GetMaterials()[RealMaterialIndex].MaterialInterface = Material;
		SkeletalMesh->PostEditChange();
	}
}

AActor* UBPFL::SpawnGenericActorWithoutRoot()
{
    auto WorldContext = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
    auto World = WorldContext->World();
    return World->SpawnActor<AActor>();
}

USceneComponent* UBPFL::AddComponentToActor(AActor* Actor, TSubclassOf<USceneComponent> ComponentClass)
{
	if (!Actor || !ComponentClass)
		return nullptr;

	USceneComponent* NewComponent = NewObject<USceneComponent>(Actor, ComponentClass);
	if (!NewComponent)
		return nullptr;

	NewComponent->RegisterComponent();
	Actor->AddInstanceComponent(NewComponent);

	if (USceneComponent* RootComponent = Actor->GetRootComponent())
		NewComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	return NewComponent;
}

TArray<FVector3f> UBPFL::GetStaticMeshVerts(UStaticMesh* Mesh)
{
	TArray<FVector3f> ReturnArray;
	if (Mesh->GetRenderData()->LODResources.Num() > 0)
	{
		if (FPositionVertexBuffer* VertexBuffer = &Mesh->GetRenderData()->LODResources[0].VertexBuffers.PositionVertexBuffer)
			for (uint32 Index = 0; Index < VertexBuffer->GetNumVertices(); Index++)
				ReturnArray.Add(VertexBuffer->VertexPosition(Index));
	}
	return ReturnArray;
}

TMap<FVector3f, FColor> UBPFL::MakeHashmap(TArray<FVector3f> VertexArray, TArray<FColor> ColorArray)
{
	TMap<FVector3f, FColor> HashMap;
	for (auto i = 0; i < VertexArray.Num(); i++)
		HashMap.Add(VertexArray[i], ColorArray[i]);
	return HashMap;
}

void UBPFL::OverrideVertexColors(UStaticMeshComponent* Component, TArray<FString> Data, FString FilePath)
{
	if (UStaticMesh* StaticMesh = Component->GetStaticMesh())
	{
		Component->SetLODDataCount(1, Component->LODData.Num());
		FStaticMeshComponentLODInfo* LODInfo = &Component->LODData[0];
		LODInfo->PaintedVertices.Empty();
		LODInfo->OverrideVertexColors = new FColorVertexBuffer();

		//get Vertex Positions from UEModel file
		UEFModelReader UEModel = UEFModelReader(FilePath);
		if (!UEModel.Read())
			return;

		//read Override Colors
		TArray<FColor> OverrideColors;
		for (auto Hex : Data)
			OverrideColors.Add(FColor::FromHex(Hex));

		//make map between Position and Color
		auto OriginalPosToColorMap = MakeHashmap(UEModel.LODs[0].Vertices, OverrideColors);

		TArray<FVector3f> CurrentVerts = GetStaticMeshVerts(StaticMesh);
		TArray<FColor> FinalColors;

		//find matching Color for imported Position and add it to final Colors
		for (auto& Vertex : CurrentVerts)
		{
			Vertex.Y = -Vertex.Y; //invert Y like UEModelFactory does
			auto Found = OriginalPosToColorMap.Find(Vertex);
			FinalColors.Add(*Found);
		}

		LODInfo->OverrideVertexColors->InitFromColorArray(FinalColors);
		BeginInitResource(LODInfo->OverrideVertexColors);
	}
}