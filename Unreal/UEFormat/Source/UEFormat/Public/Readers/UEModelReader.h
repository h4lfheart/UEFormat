#pragma once
#include <fstream>

template<typename T>
void ReadArray(std::ifstream& Ar, int ArraySize, TArray<T>& Data) {
	Data.SetNum(ArraySize);
	for (auto i = 0; i < ArraySize; i++) {
		Ar.read(reinterpret_cast<char*>(&Data[i]), sizeof(T));
	}
}

template<typename T>
T ReadData(std::ifstream& Ar) {
	T Data;
	Ar.read(reinterpret_cast<char*>(&Data), sizeof(T));
	return Data;
}

FQuat4f ReadQuat(std::ifstream& Ar)
{
	float X = ReadData<float>(Ar);
	float Y = ReadData<float>(Ar);
	float Z = ReadData<float>(Ar);
	float W = ReadData<float>(Ar);
	auto Data = FQuat4f(X, Y, Z, W);
	return Data;
}

std::string ReadString(std::ifstream& Ar, int32 Size) {
	std::string String;
	String.resize(Size);
	Ar.read(&String[0], Size);  // Read the data directly into the strings buffer
	return String;
}

std::string ReadFString(std::ifstream& Ar) {
	int32 Size = ReadData<int32>(Ar);
	std::string String;
	String.resize(Size);
	Ar.read(&String[0], Size);
	return String;
}

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
class UEModelReader
{
public:
	UEModelReader(const FString Filename);
	bool Read();
	void ReadArchive(std::ifstream& Archive);

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





