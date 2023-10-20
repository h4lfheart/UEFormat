#include "Readers/UEModelReader.h"
#include "Misc/Compression.h"

UEModelReader::UEModelReader(const FString Filename) {
	Ar.open(ToCStr(Filename), std::ios::binary);
}

bool UEModelReader::Read() {
	std::string Magic = ReadString(Ar, GMAGIC.length());
	if (Magic != GMAGIC) return false;

	Header.Identifier = ReadFString(Ar);
	Header.FileVersionBytes = ReadData<std::byte>(Ar);
	Header.ObjectName = ReadFString(Ar);
	Header.IsCompressed = ReadData<bool>(Ar);
	
	if (Header.IsCompressed) {
		Header.CompressionType = ReadFString(Ar);
		Header.CompressedSize = ReadData<int32>(Ar);
		Header.UncompressedSize = ReadData<int32>(Ar);

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
		Chunk.ArraySize = ReadData<int32>(Archive);
		Chunk.ByteSize = ReadData<int32>(Archive);

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
				Materials[i].FirstIndex = ReadData<int32>(Archive);
				Materials[i].NumFaces = ReadData<int32>(Archive);
			}
		}
		else if (Chunk.HeaderName == "TEXCOORDS") {
			TextureCoordinates.SetNum(Chunk.ArraySize);
			for (auto i = 0; i < Chunk.ArraySize; i++) {
				int32 UVCount = ReadData<int32>(Archive);
				TextureCoordinates[i].SetNum(UVCount);
				for (auto j = 0; j < UVCount; j++) {
					float U = ReadData<float>(Archive);
					float V = ReadData<float>(Archive);
					TextureCoordinates[i][j] = FVector2f(U, V);
				}
			}
		}
		else if (Chunk.HeaderName == "SOCKETS") {
			Sockets.SetNum(Chunk.ArraySize);
			for (auto i = 0; i < Chunk.ArraySize; i++) {
				Sockets[i].SocketName = ReadFString(Archive);
				Sockets[i].SocketParentName = ReadFString(Archive);
				Sockets[i].SocketPos = ReadData<FVector3f>(Archive);
				Sockets[i].SocketRot = ReadQuat(Archive);
				Sockets[i].SocketScale = ReadData<FVector3f>(Archive);
			}
		}
		else if (Chunk.HeaderName == "BONES") {
			Bones.SetNum(Chunk.ArraySize);
			for (auto i = 0; i < Chunk.ArraySize; i++) {
				Bones[i].BoneName = ReadFString(Archive);
				Bones[i].BoneParentIndex = ReadData<int32>(Archive);
				Bones[i].BonePos = ReadData<FVector3f>(Archive);
				Bones[i].BoneRot = ReadQuat(Archive);
			}
		}
		else if (Chunk.HeaderName == "WEIGHTS") {
			Weights.SetNum(Chunk.ArraySize);
			for (auto i = 0; i < Chunk.ArraySize; i++) {
				Weights[i].WeightBoneIndex = ReadData<short>(Archive);
				Weights[i].WeightVertexIndex = ReadData<int32>(Archive);
				Weights[i].WeightAmount = ReadData<float>(Archive);
			}
		}
		else if (Chunk.HeaderName == "MORPHTARGETS") {
			Morphs.SetNum(Chunk.ArraySize);
			for (auto i = 0; i < Chunk.ArraySize; i++) {
				Morphs[i].MorphName = ReadFString(Archive);
				auto DeltaNum = ReadData<int32>(Archive);

				Morphs[i].MorphDeltas.SetNum(DeltaNum);
				for (auto j = 0; j < DeltaNum; j++) {
					Morphs[i].MorphDeltas[j].MorphPosition = ReadData<FVector3f>(Archive);
					Morphs[i].MorphDeltas[j].MorphNormals = ReadData<FVector3f>(Archive);
					Morphs[i].MorphDeltas[j].MorphVertexIndex = ReadData<int32>(Archive);
				}
			}
		}
		else {
			Archive.ignore(Chunk.ByteSize);
		}
	}
	Archive.close();
}