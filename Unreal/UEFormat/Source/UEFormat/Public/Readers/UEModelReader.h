#pragma once
#include <fstream>

struct FMaterialChunk
{
	int32 MatIndex;
	std::string MaterialName;
	int32 FirstIndex;
	int32 NumFaces;
};
struct FUnrealFormatHeader
{
	std::string Identifier;
	std::byte FileVersionBytes;
	std::string ObjectName;
	bool IsCompressed;
	std::string CompressionType;
	int32 CompressedSize;
	int32 UncompressedSize;
};
struct FDataChunk
{
	std::string HeaderName;
	int32 ArraySize;
	int32 ByteSize;
};
class FMeshFloatUV
{
public:
	FMeshFloatUV(float A = 0.0f, float B = 0.0f)
		: U(A), V(B)
	{
	}
	float U;
	float V;
};
class UEModelReader
{
public:
	UEModelReader(const FString Filename);
	bool Read();
	
	const std::string GMAGIC = "UNREALFORMAT";

	bool bHasVertexNormals;
	bool bHasVertexColors;
	bool bHasExtraUVs;

	TArray<FVector3f> Vertices;
	TArray<int32> Indices;
	TArray<FVector3f> Normals;
	TArray<FVector3f> Tangents;
	TArray<FColor> VertexColors;
	TArray<TArray<FMeshFloatUV>> TextureCoordinates;
	TArray<FMaterialChunk> Materials;

private:
	std::ifstream Ar;
};





