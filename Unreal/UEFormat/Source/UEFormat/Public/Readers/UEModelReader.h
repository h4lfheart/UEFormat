#pragma once
#include <fstream>

struct FWeightChunk
{
	short WeightBoneIndex;
	int32 WeightVertexIndex;
	float WeightAmount;
};
struct FBoneChunk
{
	std::string BoneName;
	int32 BoneParentIndex;
	FVector3f BonePos;
	FQuat4f BoneRot;
};
struct FSocketChunk
{
	std::string SocketName;
	std::string SocketParentName;
	FVector3f SocketPos;
	FQuat4f SocketRot;
	FVector3f SocketScale;
};
struct FMaterialChunk
{
	int32 MatIndex;
	std::string Name;
	int32 FirstIndex;
	int32 NumFaces;
};
struct FMorphTargetDataChunk
{
	FVector3f MorphPosition;
	FVector3f MorphNormals;
	int32 MorphVertexIndex;
};
struct FMorphTargetChunk
{
	std::string MorphName;
	TArray<FMorphTargetDataChunk> MorphDeltas;
};
struct FUEFormatHeader
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
class UEModelReader
{
public:
	UEModelReader(const FString Filename);
	bool Read();
	void ReadDataFromArchive(std::ifstream& Archive);

	const std::string GMAGIC = "UEFORMAT";
	const std::string GZIP = "GZIP";
	const std::string ZSTD = "ZSTD";

	FUEFormatHeader Header;
	TArray<FVector3f> Vertices;
	TArray<int32> Indices;
	TArray<FVector3f> Normals;
	TArray<FVector3f> Tangents;
	TArray<FColor> VertexColors;
	TArray<TArray<FVector2f>> TextureCoordinates;
	TArray<FMaterialChunk> Materials;
	TArray<FBoneChunk> Bones;
	TArray<FSocketChunk> Sockets;
	TArray<FWeightChunk> Weights;
	TArray<FMorphTargetChunk> Morphs;

private:
	std::ifstream Ar;
};





