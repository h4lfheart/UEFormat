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
	ReadArchive(Ar);
	return true;
}

void UEModelReader::ReadArchive(std::ifstream& Archive) {
	while (!Archive.eof()) {

		std::string ChunkName = ReadFString(Archive);
		const int32 ArraySize = ReadData<int32>(Archive);
		const int32 ByteSize = ReadData<int32>(Archive);

		if (ChunkName == "VERTICES") { ReadArray(Archive, ArraySize, Vertices); }
		else if (ChunkName == "INDICES") { ReadArray(Archive, ArraySize, Indices); }
		else if (ChunkName == "NORMALS") { ReadArray(Archive, ArraySize, Normals); }
		else if (ChunkName == "TANGENTS") { ReadArray(Archive, ArraySize, Tangents); }
		else if (ChunkName == "VERTEXCOLORS") {ReadArray(Archive, ArraySize, VertexColors); }
		else if (ChunkName == "MATERIALS") {
			Materials.SetNum(ArraySize);
			for (auto i = 0; i < ArraySize; i++) {
				Materials[i].MatIndex = i;
				Materials[i].Name = ReadFString(Archive);
				Materials[i].FirstIndex = ReadData<int32>(Archive);
				Materials[i].NumFaces = ReadData<int32>(Archive);
			}
		}
		else if (ChunkName == "TEXCOORDS") {
			TextureCoordinates.SetNum(ArraySize);
			for (auto i = 0; i < ArraySize; i++) {
				int32 UVCount = ReadData<int32>(Archive);
				TextureCoordinates[i].SetNum(UVCount);
				for (auto j = 0; j < UVCount; j++) {
					float U = ReadData<float>(Archive);
					float V = ReadData<float>(Archive);
					TextureCoordinates[i][j] = FVector2f(U, V);
				}
			}
		}
		else if (ChunkName == "SOCKETS") {
			Sockets.SetNum(ArraySize);
			for (auto i = 0; i < ArraySize; i++) {
				Sockets[i].SocketName = ReadFString(Archive);
				Sockets[i].SocketParentName = ReadFString(Archive);
				Sockets[i].SocketPos = ReadData<FVector3f>(Archive);
				Sockets[i].SocketRot = ReadQuat(Archive);
				Sockets[i].SocketScale = ReadData<FVector3f>(Archive);
			}
		}
		else if (ChunkName == "BONES") {
			Bones.SetNum(ArraySize);
			for (auto i = 0; i < ArraySize; i++) {
				Bones[i].BoneName = ReadFString(Archive);
				Bones[i].BoneParentIndex = ReadData<int32>(Archive);
				Bones[i].BonePos = ReadData<FVector3f>(Archive);
				Bones[i].BoneRot = ReadQuat(Archive);
			}
		}
		else if (ChunkName == "WEIGHTS") {
			Weights.SetNum(ArraySize);
			for (auto i = 0; i < ArraySize; i++) {
				Weights[i].WeightBoneIndex = ReadData<short>(Archive);
				Weights[i].WeightVertexIndex = ReadData<int32>(Archive);
				Weights[i].WeightAmount = ReadData<float>(Archive);
			}
		}
		else if (ChunkName == "MORPHTARGETS") {
			Morphs.SetNum(ArraySize);
			for (auto i = 0; i < ArraySize; i++) {
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
			Archive.ignore(ByteSize);
		}
	}
	Archive.close();
}