#include "../Public/Readers/UEModelReader.h"


UEModelReader::UEModelReader(const FString Filename)
{
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

bool UEModelReader::Read()
{
	std::string Magic = ReadString(Ar, GMAGIC.length());
	if (Magic != GMAGIC) return false;
	
	FUnrealFormatHeader Header;
	Header.Identifier = ReadFString(Ar);
	Header.FileVersionBytes = ReadByte(Ar);
	Header.ObjectName = ReadFString(Ar);
	Header.IsCompressed = ReadBool(Ar);
	
	if (Header.IsCompressed)
	{
		Header.CompressionType = ReadFString(Ar);
		Header.CompressedSize = ReadInt(Ar);
		Header.UncompressedSize = ReadInt(Ar);
	}
	
	while (!Ar.eof())
	{
		FDataChunk Chunk;
		Chunk.HeaderName = ReadFString(Ar);
		Chunk.ArraySize = ReadInt(Ar);
		Chunk.ByteSize = ReadInt(Ar);
		
		if (Chunk.HeaderName == "VERTICES") { ReadArray(Ar, Chunk.ArraySize, Vertices); }
		else if (Chunk.HeaderName == "INDICES") { ReadArray(Ar, Chunk.ArraySize, Indices); }
		else if (Chunk.HeaderName == "NORMALS") { ReadArray(Ar, Chunk.ArraySize, Normals); }
		else if (Chunk.HeaderName == "TANGENTS") { ReadArray(Ar, Chunk.ArraySize, Tangents); }
		else if (Chunk.HeaderName == "VERTEXCOLORS") { ReadArray(Ar, Chunk.ArraySize, VertexColors); }
		else if (Chunk.HeaderName == "MATERIALS")
		{
			Materials.SetNum(Chunk.ArraySize);
			for (auto i = 0; i < Chunk.ArraySize; i++)
			{
				Materials[i].MatIndex = i;
				Materials[i].MaterialName = ReadFString(Ar);
				Materials[i].FirstIndex = ReadInt(Ar);
				Materials[i].NumFaces = ReadInt(Ar);
			}
		}
		else if (Chunk.HeaderName == "TEXCOORDS")
		{
			TextureCoordinates.SetNum(Chunk.ArraySize);
			for (auto i = 0; i < Chunk.ArraySize; i++)
			{
				int32 UVCount = ReadInt(Ar);
				TextureCoordinates[i].SetNum(UVCount);
				for (auto j = 0; j < UVCount; j++)
				{
					float U = ReadFloat(Ar);
					float V = ReadFloat(Ar);
					FMeshFloatUV UVElement(U, V);
					TextureCoordinates[i][j] = UVElement;
				}
			}
		}
		else
		{
			Ar.ignore(Chunk.ByteSize);
		}
	}
	
	bHasVertexNormals = Normals.Num() > 0;
	bHasVertexColors = VertexColors.Num() > 0;
	bHasExtraUVs = TextureCoordinates.Num() > 1;

	Ar.close();
	return true;
}
