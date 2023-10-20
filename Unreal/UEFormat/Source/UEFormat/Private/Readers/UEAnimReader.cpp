#include "Readers/UEAnimReader.h"
#include "Misc/Compression.h"
#include "Readers/UEModelReader.h"

UEAnimReader::UEAnimReader(const FString Filename) {
	Ar.open(ToCStr(Filename), std::ios::binary);
}

bool UEAnimReader::Read() {
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

	NumFrames = ReadData<int32>(Ar);
	FramesPerSecond = ReadData<int32>(Ar);

	while (!Ar.eof()) {
		std::string ChunkName = ReadFString(Ar);
		const int32 ArraySize = ReadData<int32>(Ar);
		const int32 ByteSize = ReadData<int32>(Ar);

		if (ChunkName == "TRACKS")
		{
			Tracks.SetNum(ArraySize);
			for (auto i = 0; i < ArraySize; i++) {
				Tracks[i].TrackName = ReadFString(Ar);
				const int32 PosArraySize = ReadData<int32>(Ar);
				Tracks[i].TrackPosKeys.SetNum(PosArraySize);
				for (auto j = 0; j < PosArraySize; j++) {
					Tracks[i].TrackPosKeys[j].Frame = ReadData<int32>(Ar);
					Tracks[i].TrackPosKeys[j].VectorValue = ReadData<FVector3f>(Ar);
				}
				const int32 RotArraySize = ReadData<int32>(Ar);
				Tracks[i].TrackRotKeys.SetNum(RotArraySize);
				for (auto k = 0; k < RotArraySize; k++) {
					Tracks[i].TrackRotKeys[k].Frame = ReadData<int32>(Ar);
					Tracks[i].TrackRotKeys[k].QuatValue = ReadQuat(Ar);
				}
				const int32 ScaleArraySize = ReadData<int32>(Ar);
				Tracks[i].TrackPosKeys.SetNum(ScaleArraySize);
				for (auto l = 0; l < ScaleArraySize; l++) {
					Tracks[i].TrackPosKeys[l].Frame = ReadData<int32>(Ar);
					Tracks[i].TrackPosKeys[l].VectorValue = ReadData<FVector3f>(Ar);
				}
			}
		}
		else if (ChunkName == "CURVES")
		{
			Curves.SetNum(ArraySize);
			for (auto i = 0; i < ArraySize; i++) {
				Curves[i].CurveName = ReadFString(Ar);
				const int32 KeyArraySize = ReadData<int32>(Ar);
				Curves[i].CurveKeys.SetNum(KeyArraySize);
				for (auto j = 0; j < KeyArraySize; j++) {
					Curves[i].CurveKeys[j].Frame = ReadData<int32>(Ar);
					Curves[i].CurveKeys[j].FloatValue = ReadData<float>(Ar);
				}
			}
		}
		else {
			Ar.ignore(ByteSize);
		}
	}
	Ar.close();
	return true;
}