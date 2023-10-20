#pragma once
#include <fstream>
#include "UEModelReader.h"

struct FFloatKey
{
	int32 Frame;
	float FloatValue;
};
struct FVectorKey
{
	int32 Frame;
	FVector3f VectorValue;
};
struct FQuatKey
{
	int32 Frame;
	FQuat4f QuatValue;
};
struct FCurve
{
	std::string CurveName;
	TArray<FFloatKey> CurveKeys;
};
struct FTrack
{
	std::string TrackName;
	TArray<FVectorKey> TrackPosKeys;
	TArray<FQuatKey> TrackRotKeys;
	TArray<FVectorKey> TrackScaleKeys;
};

class UEAnimReader
{
public:
	UEAnimReader(const FString Filename);
	bool Read();

	const std::string GMAGIC = "UEFORMAT";
	const std::string GZIP = "GZIP";
	const std::string ZSTD = "ZSTD";
	const std::string ANIM_IDENTIFIER = "UEANIM";

	FUEFormatHeader Header;
	int32 NumFrames;
	int32 FramesPerSecond;
	TArray<FTrack> Tracks;
	TArray<FCurve> Curves;

private:
	std::ifstream Ar;
};

