#include "harness/harness.h"

#include <cmath>

#include "ueformat/anim/anim.h"
#include "ueformat/generic/context.h"
#include "ueformat/generic/header.h"
#include "ueformat/generic/save_options.h"
#include "ueformat/model/model.h"
#include "ueformat/pose/pose.h"

namespace
{
    using namespace UEFormat;

    bool NearlyEqual(f32 a, f32 b, f32 epsilon = 1e-5f)
    {
        return std::fabs(a - b) <= epsilon;
    }

    bool VectorsEqual(const FVector& a, const FVector& b)
    {
        return NearlyEqual(a.X, b.X) && NearlyEqual(a.Y, b.Y) && NearlyEqual(a.Z, b.Z);
    }
}

UEFORMAT_TEST(lib, model_roundtrip)
{
    using namespace UEFormat;
    using Test::Check;

    UEModel model;
    UEModelLOD lod;
    lod.Name = "LOD0";
    lod.Vertices = {{1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f}};
    lod.Indices = {0, 1, 0};
    model.LODs.push_back(std::move(lod));

    Context context;
    const auto bytes = context.Save(model, {
        .ObjectName = "TestMesh",
        .ObjectPath = "/Game/Meshes/TestMesh",
    });
    const auto loaded = context.Load(bytes);

    Check(loaded.Header.Identifier == ModelIdentifier, "round-tripped header should identify as UEMODEL");
    Check(loaded.Header.ObjectName == "TestMesh", "round-tripped ObjectName should match the save options");
    Check(loaded.Header.ObjectPath == "/Game/Meshes/TestMesh", "round-tripped ObjectPath should match the save options");
    Check(loaded.Header.FileVersion == EUEFormatVersion::LatestVersion, "saved assets should always write LatestVersion");
    Check(!loaded.Header.IsCompressed, "v1 saves should leave IsCompressed false");

    const auto& out = std::get<UEModel>(loaded.Object);
    Check(out.LODs.size() == 1, "LOD list should survive round-trip with one entry");
    Check(out.LODs[0].Name == "LOD0", "LOD name should survive round-trip");
    Check(out.LODs[0].Vertices.size() == 2, "vertex array size should survive round-trip");
    Check(VectorsEqual(out.LODs[0].Vertices[0], {1.0f, 2.0f, 3.0f}), "first vertex position should survive round-trip");
    Check(out.LODs[0].Indices == TArray<u32>{0, 1, 0}, "index buffer should survive round-trip");
    Check(!out.Skeleton.has_value(), "omitted optional SKELETON section should reload as nullopt");
}

UEFORMAT_TEST(lib, model_skeleton_roundtrip)
{
    using namespace UEFormat;
    using Test::Check;

    UEModel model;
    UEModelLOD lod;
    lod.Name = "LOD0";
    lod.Vertices = {{0.0f, 0.0f, 0.0f}};
    model.LODs.push_back(std::move(lod));

    UEModelSkeleton skeleton;
    skeleton.Metadata.Path = "/Game/Skeleton";
    skeleton.Bones.push_back({.BoneName = "root", .ParentIndex = -1});
    model.Skeleton = std::move(skeleton);

    Context context;
    const auto bytes = context.Save(model, {.ObjectName = "SkelMesh"});
    const auto loaded = context.Load(bytes);
    const auto& out = std::get<UEModel>(loaded.Object);

    Check(out.Skeleton.has_value(), "present SKELETON section should reload as a populated optional");
    Check(out.Skeleton->Metadata.Path == "/Game/Skeleton", "skeleton metadata path should survive round-trip");
    Check(out.Skeleton->Bones.size() == 1, "skeleton bone array size should survive round-trip");
    Check(out.Skeleton->Bones[0].BoneName == "root", "root bone name should survive round-trip");
}

UEFORMAT_TEST(lib, anim_roundtrip)
{
    using namespace UEFormat;
    using Test::Check;

    UEAnim anim;
    anim.Metadata = {
        .NumFrames = 10,
        .FramesPerSecond = 30.0f,
        .RefPosePath = "/Game/Ref",
    };
    anim.Tracks.push_back({
        .BoneName = "pelvis",
        .PositionKeys = {{0, {1.0f, 0.0f, 0.0f}}},
    });
    anim.Curves.push_back({
        .CurveName = "blink",
        .Keys = {{0, 0.5f}},
    });

    Context context;
    const auto bytes = context.Save(anim, {
        .ObjectName = "TestAnim",
        .ObjectPath = "/Game/Anims/TestAnim",
    });
    const auto loaded = context.Load(bytes);

    Check(loaded.Header.Identifier == AnimIdentifier, "round-tripped header should identify as UEANIM");
    const auto& out = std::get<UEAnim>(loaded.Object);
    Check(out.Metadata.NumFrames == 10, "anim NumFrames should survive round-trip");
    Check(NearlyEqual(out.Metadata.FramesPerSecond, 30.0f), "anim FramesPerSecond should survive round-trip");
    Check(out.Tracks.size() == 1, "track array size should survive round-trip");
    Check(out.Tracks[0].BoneName == "pelvis", "track bone name should survive round-trip");
    Check(out.Curves.size() == 1, "curve array size should survive round-trip");
    Check(NearlyEqual(out.Curves[0].Keys[0].Value, 0.5f), "first curve key value should survive round-trip");
}

UEFORMAT_TEST(lib, pose_roundtrip)
{
    using namespace UEFormat;
    using Test::Check;

    UEPose pose;
    pose.Poses.push_back({
        .PoseName = "smile",
        .Keys = {{.BoneName = "jaw", .Location = {0.0f, 1.0f, 0.0f}}},
    });
    pose.CurveNames = {"smile_curve"};

    Context context;
    const auto bytes = context.Save(pose, {.ObjectName = "TestPose"});
    const auto loaded = context.Load(bytes);

    Check(loaded.Header.Identifier == PoseIdentifier, "round-tripped header should identify as UEPOSE");
    const auto& out = std::get<UEPose>(loaded.Object);
    Check(out.Poses.size() == 1, "pose array size should survive round-trip");
    Check(out.Poses[0].PoseName == "smile", "pose name should survive round-trip");
    Check(out.Poses[0].Keys[0].BoneName == "jaw", "pose key bone name should survive round-trip");
    Check(VectorsEqual(out.Poses[0].Keys[0].Location, {0.0f, 1.0f, 0.0f}), "pose key location should survive round-trip");
    Check(out.CurveNames.size() == 1 && out.CurveNames[0] == "smile_curve", "curve name list should survive round-trip");
}

UEFORMAT_TEST(lib, model_zstd_roundtrip)
{
    using namespace UEFormat;
    using Test::Check;

    UEModel model;
    UEModelLOD lod;
    lod.Name = "LOD0";
    lod.Vertices = {{1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f}};
    lod.Indices = {0, 1, 0};
    model.LODs.push_back(std::move(lod));

    Context context;
    const auto bytes = context.Save(model, {
        .ObjectName = "ZstdMesh",
        .ObjectPath = "/Game/Meshes/ZstdMesh",
        .Compression = EFileCompressionFormat::ZSTD,
        .CompressionLevel = 6,
    });
    const auto loaded = context.Load(bytes);

    Check(loaded.Header.IsCompressed, "ZSTD save should set IsCompressed");
    Check(loaded.Header.CompressionFormat == "ZSTD", "compression format should be ZSTD");
    Check(loaded.Header.UncompressedSize > 0, "uncompressed size should be recorded");
    Check(loaded.Header.CompressedSize > 0, "compressed size should be recorded");
    Check(loaded.Header.CompressedSize <= loaded.Header.UncompressedSize, "compressed payload should not exceed uncompressed size for this fixture");

    const auto& out = std::get<UEModel>(loaded.Object);
    Check(out.LODs.size() == 1, "ZSTD LOD list should survive round-trip");
    Check(VectorsEqual(out.LODs[0].Vertices[0], {1.0f, 2.0f, 3.0f}), "ZSTD vertex data should survive round-trip");
}

UEFORMAT_TEST(lib, model_gzip_roundtrip)
{
    using namespace UEFormat;
    using Test::Check;

    UEModel model;
    UEModelLOD lod;
    lod.Name = "LOD0";
    lod.Vertices = {{9.0f, 8.0f, 7.0f}};
    lod.Indices = {0, 0, 0};
    model.LODs.push_back(std::move(lod));

    Context context;
    const auto bytes = context.Save(model, {
        .ObjectName = "GzipMesh",
        .Compression = EFileCompressionFormat::GZIP,
        .CompressionLevel = 6,
    });
    const auto loaded = context.Load(bytes);

    Check(loaded.Header.IsCompressed, "GZIP save should set IsCompressed");
    Check(loaded.Header.CompressionFormat == "GZIP", "compression format should be GZIP");

    const auto& out = std::get<UEModel>(loaded.Object);
    Check(out.LODs[0].Name == "LOD0", "GZIP LOD name should survive round-trip");
    Check(VectorsEqual(out.LODs[0].Vertices[0], {9.0f, 8.0f, 7.0f}), "GZIP vertex data should survive round-trip");
}
