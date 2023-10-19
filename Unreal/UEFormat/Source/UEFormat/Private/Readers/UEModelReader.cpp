#include "../Public/Readers/UEModelReader.h"
#include "Misc/Compression.h"

UEModelReader::UEModelReader(const FString Filename) {
	Ar.open(ToCStr(Filename), std::ios::binary);
}

template<typename T>
void ReadArray(std::ifstream& Ar, int arraySize, TArray<T>& data) {
	data.SetNum(arraySize);
	for (auto i = 0; i < arraySize; i++) {
		Ar.read(reinterpret_cast<char*>(&data[i]), sizeof(T));
	}
}

bool ReadBool(std::ifstream& Ar)
{
	char Buffer[1];
	Ar.read(Buffer, 1);
	return static_cast<bool>(Buffer[0]);
}

std::byte ReadByte(std::ifstream& Ar)
{
	char Buffer[8];
	Ar.read(Buffer, 1);
	return static_cast<std::byte>(Buffer[0]);
}

int32 ReadInt(std::ifstream& Ar)
{
	int32 Out;
	Ar.read(reinterpret_cast<char*>(&Out), sizeof(int32));
	return Out;
}

float ReadFloat(std::ifstream& Ar)
{
	float Out;
	Ar.read(reinterpret_cast<char*>(&Out), sizeof(float));
	return Out;
}

short ReadShort(std::ifstream& Ar)
{
	float Out;
	Ar.read(reinterpret_cast<char*>(&Out), sizeof(short));
	return Out;
}

FVector3f ReadVector(std::ifstream& Ar)
{
	FVector3f Out;
	Ar.read(reinterpret_cast<char*>(&Out), sizeof(FVector3f));
	return Out;
}

FQuat4f ReadQuat(std::ifstream& Ar)
{
	float X = ReadFloat(Ar);
	float Y = ReadFloat(Ar);
	float Z = ReadFloat(Ar);
	float W = ReadFloat(Ar);
	FQuat4f Out = FQuat4f(X,Y,Z,W);
	return Out;
}

std::string ReadString(std::ifstream& Ar, int32 Size) {
	std::string String;
	String.resize(Size);
	Ar.read(&String[0], Size);  // Read the data directly into the strings buffer
	return String;
}

std::string ReadFString(std::ifstream& Ar) {
	int32 Size = ReadInt(Ar);
	std::string String;
	String.resize(Size);
	Ar.read(&String[0], Size);
	return String;
}

bool UEModelReader::Read() {
	std::string Magic = ReadString(Ar, GMAGIC.length());
	if (Magic != GMAGIC) return false;

	Header.Identifier = ReadFString(Ar);
	Header.FileVersionBytes = ReadByte(Ar);
	Header.ObjectName = ReadFString(Ar);
	Header.IsCompressed = ReadBool(Ar);
	
	if (Header.IsCompressed) {
		Header.CompressionType = ReadFString(Ar);
		Header.CompressedSize = ReadInt(Ar);
		Header.UncompressedSize = ReadInt(Ar);

		char* UncompressedBuffer = new char[Header.UncompressedSize];
		char* CompressedBuffer = new char[Header.CompressedSize];
		Ar.read(CompressedBuffer, Header.CompressedSize);

		if (Header.CompressionType == GZIP) {
			FCompression::UncompressMemory(FName("NAME_Gzip"), UncompressedBuffer, Header.UncompressedSize, CompressedBuffer, Header.CompressedSize);
		}
		else if (Header.CompressionType == ZSTD) {
			FCompression::UncompressMemory(FName("NAME_Zlib"), UncompressedBuffer, Header.UncompressedSize, CompressedBuffer, Header.CompressedSize);
		}
	}
	ReadDataFromArchive(Ar);
	return true;
}

void UEModelReader::ReadDataFromArchive(std::ifstream& Archive) {
	while (!Archive.eof()) {
		FDataChunk Chunk;
		Chunk.HeaderName = ReadFString(Archive);
		Chunk.ArraySize = ReadInt(Archive);
		Chunk.ByteSize = ReadInt(Archive);

		if (Chunk.HeaderName == "VERTICES") { ReadArray(Archive, Chunk.ArraySize, Vertices); }
		else if (Chunk.HeaderName == "INDICES") { ReadArray(Archive, Chunk.ArraySize, Indices); }
		else if (Chunk.HeaderName == "NORMALS") { ReadArray(Archive, Chunk.ArraySize, Normals); }
		else if (Chunk.HeaderName == "TANGENTS") { ReadArray(Archive, Chunk.ArraySize, Tangents); }
		else if (Chunk.HeaderName == "VERTEXCOLORS") {ReadArray(Archive, Chunk.ArraySize, VertexColors); }
		else if (Chunk.HeaderName == "MATERIALS") {
			Materials.SetNum(Chunk.ArraySize);
			for (auto i = 0; i < Chunk.ArraySize; i++) {
				Materials[i].MatIndex = i;
				Materials[i].Name = ReadFString(Archive);
				Materials[i].FirstIndex = ReadInt(Archive);
				Materials[i].NumFaces = ReadInt(Archive);
			}
		}
		else if (Chunk.HeaderName == "TEXCOORDS") {
			TextureCoordinates.SetNum(Chunk.ArraySize);
			for (auto i = 0; i < Chunk.ArraySize; i++) {
				int32 UVCount = ReadInt(Archive);
				TextureCoordinates[i].SetNum(UVCount);
				for (auto j = 0; j < UVCount; j++) {
					float U = ReadFloat(Archive);
					float V = ReadFloat(Archive);
					TextureCoordinates[i][j] = FVector2f(U, V);
				}
			}
		}
		else if (Chunk.HeaderName == "SOCKETS") {
			Sockets.SetNum(Chunk.ArraySize);
			for (auto i = 0; i < Chunk.ArraySize; i++) {
				Sockets[i].SocketName = ReadFString(Archive);
				Sockets[i].SocketParentName = ReadFString(Archive);
				Sockets[i].SocketPos = ReadVector(Archive);
				Sockets[i].SocketRot = ReadQuat(Archive);
				Sockets[i].SocketScale = ReadVector(Archive);
			}
		}
		else if (Chunk.HeaderName == "BONES") {
			Bones.SetNum(Chunk.ArraySize);
			for (auto i = 0; i < Chunk.ArraySize; i++) {
				Bones[i].BoneName = ReadFString(Archive);
				Bones[i].BoneParentIndex = ReadInt(Archive);
				Bones[i].BonePos = ReadVector(Archive);
				Bones[i].BoneRot = ReadQuat(Archive);
			}
		}
		else if (Chunk.HeaderName == "WEIGHTS") {
			Weights.SetNum(Chunk.ArraySize);
			for (auto i = 0; i < Chunk.ArraySize; i++) {
				Weights[i].WeightBoneIndex = ReadShort(Archive);
				Weights[i].WeightVertexIndex = ReadInt(Archive);
				Weights[i].WeightAmount = ReadFloat(Archive);
			}
		}
		else if (Chunk.HeaderName == "MORPHTARGETS") {
			Morphs.SetNum(Chunk.ArraySize);
			for (auto i = 0; i < Chunk.ArraySize; i++) {
				Morphs[i].MorphName = ReadFString(Archive);
				auto DeltaNum = ReadInt(Archive);

				Morphs[i].MorphDeltas.SetNum(DeltaNum);
				for (auto j = 0; j < DeltaNum; j++) {
					Morphs[i].MorphDeltas[j].MorphPosition = ReadVector(Archive);
					Morphs[i].MorphDeltas[j].MorphNormals = ReadVector(Archive);
					Morphs[i].MorphDeltas[j].MorphVertexIndex = ReadInt(Archive);
				}
			}
		}
		else {
			Archive.ignore(Chunk.ByteSize);
		}
	}
	Archive.close();
}