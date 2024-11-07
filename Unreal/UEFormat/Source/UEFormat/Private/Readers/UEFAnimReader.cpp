#include "Readers/UEFAnimReader.h"
#include "Readers/UEFModelReader.h"
#include "Misc/Compression.h"
#include <string>
#include "zstd.h"

UEFAnimReader::UEFAnimReader(const FString Filename) {
	Ar.open(ToCStr(Filename), std::ios::binary);
}

bool UEFAnimReader::Read() {
	const std::string Magic = ReadString(Ar, GMAGIC.length());
	if (Magic != GMAGIC) return false;

	Header.Identifier = ReadFString(Ar);
	Header.FileVersionBytes = ReadData<std::byte>(Ar);
	Header.ObjectName = ReadFString(Ar);
	Header.IsCompressed = ReadData<bool>(Ar);

	if (Header.IsCompressed) {
		Header.CompressionType = ReadFString(Ar);
		Header.UncompressedSize = ReadData<int32>(Ar);
		Header.CompressedSize = ReadData<int32>(Ar);

		char* CompressedBuffer = new char[Header.CompressedSize];
		Ar.read(CompressedBuffer, Header.CompressedSize);

		char* UncompressedBuffer = new char[Header.UncompressedSize];

		if (Header.CompressionType == "ZSTD")
			ZSTD_decompress(UncompressedBuffer, Header.UncompressedSize, CompressedBuffer, Header.CompressedSize);
		else if (Header.CompressionType == "GZIP")
			FCompression::UncompressMemory(NAME_Gzip, UncompressedBuffer, Header.UncompressedSize, CompressedBuffer, Header.CompressedSize);

		ReadBuffer(UncompressedBuffer, Header.UncompressedSize);

		delete[] CompressedBuffer;
		delete[] UncompressedBuffer;
	}
	else {
		const auto CurrentPos = Ar.tellg();
		Ar.seekg(0, std::ios::end);
		const auto RemainingSize = Ar.tellg() - CurrentPos;
		Ar.seekg(CurrentPos, std::ios::beg);

		char* UncompressedBuffer = new char[RemainingSize];

		Ar.read(UncompressedBuffer, RemainingSize);
		ReadBuffer(UncompressedBuffer, RemainingSize);

		delete[] UncompressedBuffer;
	}

	Ar.close();
	return true;
}

void UEFAnimReader::ReadBuffer(const char* Buffer, int32 BufferSize)
{
	int32 Offset = 0;

	NumFrames = ReadBufferData<int32>(Buffer, Offset);
	FramesPerSecond = ReadBufferData<float>(Buffer, Offset);

	while (Offset < BufferSize)
	{
		std::string ChunkName = ReadBufferFString(Buffer, Offset);
		int32 ArraySize = ReadBufferData<int32>(Buffer, Offset);
		int32 ByteSize = ReadBufferData<int32>(Buffer, Offset);
		if (ChunkName == "TRACKS")
		{
			Tracks.SetNum(ArraySize);
			for (auto i = 0; i < ArraySize; i++)
			{
				Tracks[i].TrackName = ReadBufferFString(Buffer, Offset);

				const int32 PosArraySize = ReadBufferData<int32>(Buffer, Offset);
				Tracks[i].TrackPosKeys.SetNum(PosArraySize);
				for (auto j = 0; j < PosArraySize; j++)
				{
					Tracks[i].TrackPosKeys[j].Frame = ReadBufferData<int32>(Buffer, Offset);
					Tracks[i].TrackPosKeys[j].VectorValue = ReadBufferData<FVector3f>(Buffer, Offset);
				}

				const int32 RotArraySize = ReadBufferData<int32>(Buffer, Offset);
				Tracks[i].TrackRotKeys.SetNum(RotArraySize);
				for (auto k = 0; k < RotArraySize; k++)
				{
					Tracks[i].TrackRotKeys[k].Frame = ReadBufferData<int32>(Buffer, Offset);
					Tracks[i].TrackRotKeys[k].QuatValue = ReadBufferQuat(Buffer, Offset);
				}

				const int32 ScaleArraySize = ReadBufferData<int32>(Buffer, Offset);
				Tracks[i].TrackScaleKeys.SetNum(ScaleArraySize);
				for (auto l = 0; l < ScaleArraySize; l++)
				{
					Tracks[i].TrackScaleKeys[l].Frame = ReadBufferData<int32>(Buffer, Offset);
					Tracks[i].TrackScaleKeys[l].VectorValue = ReadBufferData<FVector3f>(Buffer, Offset);
				}
			}
		}
		else if (ChunkName == "CURVES")
		{
			Curves.SetNum(ArraySize);
			for (auto i = 0; i < ArraySize; i++)
			{
				Curves[i].CurveName = ReadBufferFString(Buffer, Offset);
				const int32 KeyArraySize = ReadBufferData<int32>(Buffer, Offset);
				Curves[i].CurveKeys.SetNum(KeyArraySize);
				for (auto j = 0; j < KeyArraySize; j++)
				{
					Curves[i].CurveKeys[j].Frame = ReadBufferData<int32>(Buffer, Offset);
					Curves[i].CurveKeys[j].FloatValue = ReadBufferData<float>(Buffer, Offset);
				}
			}
		}
		else
			Offset += ByteSize;
	}
}
