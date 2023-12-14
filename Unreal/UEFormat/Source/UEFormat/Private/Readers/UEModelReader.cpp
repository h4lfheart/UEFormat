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

		if (Header.CompressionType == "GZIP") {
			//ReadBuffer(UncompressedBuffer, Header.UncompressedSize);
		}
		if (Header.CompressionType == "ZSTD") {
			//ReadBuffer(UncompressedBuffer, Header.UncompressedSize);
		}
	}
	else
	{
		//find out remaining size
		const auto CurrentPos = Ar.tellg();
		Ar.seekg(0, std::ios::end);
		const auto EndPos = Ar.tellg();
		Ar.seekg(CurrentPos, std::ios::beg);

		const auto RemainingSize = EndPos - CurrentPos;
		char* UncompressedBuffer = new char[RemainingSize];

		Ar.read(UncompressedBuffer, RemainingSize);
		ReadBuffer(UncompressedBuffer, RemainingSize);
	}
	Ar.close();
	return true;
}

void UEModelReader::ReadBuffer(const char* Buffer, int BufferSize) {
	int offset = 0;

	while (offset < BufferSize) {
		std::string ChunkName = ReadBufferFString(Buffer, offset);
		const int32 ArraySize = ReadBufferData<int32>(Buffer, offset);
		const int32 ByteSize = ReadBufferData<int32>(Buffer, offset);

		if (ChunkName == "VERTICES") { ReadBufferArray(Buffer, offset, ArraySize, Vertices); }
		else if (ChunkName == "INDICES") { ReadBufferArray(Buffer, offset, ArraySize, Indices); }
		else if (ChunkName == "NORMALS") { ReadBufferArray(Buffer, offset, ArraySize, Normals); }
		else if (ChunkName == "TANGENTS") { ReadBufferArray(Buffer, offset, ArraySize, Tangents); }
		else if (ChunkName == "VERTEXCOLORS") {ReadBufferArray(Buffer, offset, ArraySize, VertexColors); }
		else if (ChunkName == "MATERIALS") {
			Materials.SetNum(ArraySize);
			for (auto i = 0; i < ArraySize; i++) {
				Materials[i].MatIndex = i;
				Materials[i].Name = ReadBufferFString(Buffer, offset);
				Materials[i].FirstIndex = ReadBufferData<int32>(Buffer, offset);
				Materials[i].NumFaces = ReadBufferData<int32>(Buffer, offset);
			}
		}
		else if (ChunkName == "TEXCOORDS") {
			TextureCoordinates.SetNum(ArraySize);
			for (auto i = 0; i < ArraySize; i++) {
				int32 UVCount = ReadBufferData<int32>(Buffer, offset);
				TextureCoordinates[i].SetNum(UVCount);
				for (auto j = 0; j < UVCount; j++) {
					float U = ReadBufferData<float>(Buffer, offset);
					float V = ReadBufferData<float>(Buffer, offset);
					TextureCoordinates[i][j] = FVector2f(U, 1-V);
				}
			}
		}
		else if (ChunkName == "SOCKETS") {
			Sockets.SetNum(ArraySize);
			for (auto i = 0; i < ArraySize; i++) {
				Sockets[i].SocketName = ReadBufferFString(Buffer, offset);
				Sockets[i].SocketParentName = ReadBufferFString(Buffer, offset);
				Sockets[i].SocketPos = ReadBufferData<FVector3f>(Buffer, offset);
				Sockets[i].SocketRot = ReadBufferQuat(Buffer, offset);
				Sockets[i].SocketScale = ReadBufferData<FVector3f>(Buffer, offset);
			}
		}
		else if (ChunkName == "BONES") {
			Bones.SetNum(ArraySize);
			for (auto i = 0; i < ArraySize; i++) {
				Bones[i].BoneName = ReadBufferFString(Buffer, offset);
				Bones[i].BoneParentIndex = ReadBufferData<int32>(Buffer, offset);
				Bones[i].BonePos = ReadBufferData<FVector3f>(Buffer, offset);
				Bones[i].BoneRot = ReadBufferQuat(Buffer, offset);
			}
		}
		else if (ChunkName == "WEIGHTS") {
			Weights.SetNum(ArraySize);
			for (auto i = 0; i < ArraySize; i++) {
				Weights[i].WeightBoneIndex = ReadBufferData<short>(Buffer, offset);
				Weights[i].WeightVertexIndex = ReadBufferData<int32>(Buffer, offset);
				Weights[i].WeightAmount = ReadBufferData<float>(Buffer, offset);
			}
		}
		else if (ChunkName == "MORPHTARGETS") {
			Morphs.SetNum(ArraySize);
			for (auto i = 0; i < ArraySize; i++) {
				Morphs[i].MorphName = ReadBufferFString(Buffer, offset);
				const auto DeltaNum = ReadBufferData<int32>(Buffer, offset);

				Morphs[i].MorphDeltas.SetNum(DeltaNum);
				for (auto j = 0; j < DeltaNum; j++) {
					Morphs[i].MorphDeltas[j].MorphPosition = ReadBufferData<FVector3f>(Buffer, offset);
					Morphs[i].MorphDeltas[j].MorphNormals = ReadBufferData<FVector3f>(Buffer, offset);
					Morphs[i].MorphDeltas[j].MorphVertexIndex = ReadBufferData<int32>(Buffer, offset);
				}
			}
		}
		else {
			offset += ByteSize;
		}
	}
}