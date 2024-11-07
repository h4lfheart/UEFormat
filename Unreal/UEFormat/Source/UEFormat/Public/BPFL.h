#pragma once
#include "CoreMinimal.h"
#include "Engine/StaticMesh.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPFL.generated.h"

UCLASS()
class UEFORMAT_API UBPFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = Mesh)
	static void SetMaterial(UObject* Mesh, int MaterialIndex, UMaterialInterface* Material);
	UFUNCTION(BlueprintCallable, Category = Actor)
	static AActor* SpawnGenericActorWithoutRoot();
	UFUNCTION(BlueprintCallable, Category = Actor)
	static USceneComponent* AddComponentToActor(AActor* Actor, TSubclassOf<USceneComponent> ComponentClass);

	UFUNCTION(BlueprintCallable, Category = VertexPainting)
	static void OverrideVertexColors(UStaticMeshComponent* Component, TArray<FString> Data, FString FilePath);

	static TMap<FVector3f, FColor> MakeHashmap(TArray<FVector3f> VertexArray, TArray<FColor> ColorArray);
	static TArray<FColor> FixBrokenMesh(UStaticMesh* SMesh, TArray<FColor> BrokenColorArray, TArray<FVector3f> ReaderVerts);
	static TArray<FVector3f> GetStaticMeshVerts(UStaticMesh* Mesh);

};