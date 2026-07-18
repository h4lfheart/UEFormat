#include "harness/harness.h"

#include <cmath>
#include <span>
#include <string_view>
#include <utility>

#include "ueformat/api/api.h"

namespace
{
    using UEFormat::Test::Check;

    bool NearlyEqual(float a, float b, float epsilon = 1e-5f)
    {
        return std::fabs(a - b) <= epsilon;
    }

    struct OwnedBuffer
    {
        UEFormatBuffer Buffer{};

        OwnedBuffer() = default;

        explicit OwnedBuffer(UEFormatBuffer buffer)
            : Buffer(buffer)
        {
        }

        OwnedBuffer(const OwnedBuffer&) = delete;
        OwnedBuffer& operator=(const OwnedBuffer&) = delete;

        OwnedBuffer(OwnedBuffer&& other) noexcept
            : Buffer(std::exchange(other.Buffer, {}))
        {
        }

        OwnedBuffer& operator=(OwnedBuffer&& other) noexcept
        {
            if (this != &other)
            {
                Reset();
                Buffer = std::exchange(other.Buffer, {});
            }
            return *this;
        }

        ~OwnedBuffer()
        {
            Reset();
        }

        void Reset()
        {
            ueformat_buffer_free(&Buffer);
        }
    };

    OwnedBuffer SaveChecked(UEFormatBufferResult result, std::string_view fallback)
    {
        Check(result.status == UEFORMAT_OK,
            result.error != nullptr ? result.error : fallback);
        Check(result.buffer.data != nullptr && result.buffer.size > 0, "save should return a non-empty byte buffer");
        return OwnedBuffer{result.buffer};
    }

    void RequireOk(UEFormatStatus status, const char* error, std::string_view fallback)
    {
        Check(status == UEFORMAT_OK, error != nullptr ? error : fallback);
    }
}

UEFORMAT_TEST(api, save_load_model)
{
    const UEFormatVector vertices[] = {
        {1.0f, 2.0f, 3.0f},
        {4.0f, 5.0f, 6.0f},
    };
    const uint32_t indices[] = {0, 1, 0};

    UEFormatModelLodDesc lod{};
    lod.name = "LOD0";
    lod.vertices = vertices;
    lod.vertex_count = 2;
    lod.indices = indices;
    lod.index_count = 3;

    UEFormatBoneDesc bone{};
    bone.bone_name = "root";
    bone.parent_index = -1;
    bone.orientation = {0.0f, 0.0f, 0.0f, 1.0f};

    UEFormatModelSkeletonDesc skeleton{};
    skeleton.metadata_path = "/Game/Skeleton";
    skeleton.bones = &bone;
    skeleton.bone_count = 1;

    UEFormatModelDesc desc{};
    desc.lods = &lod;
    desc.lod_count = 1;
    desc.skeleton = &skeleton;

    const UEFormatSaveOptions options{
        .object_name = "ApiMesh",
        .object_path = "/Game/Meshes/ApiMesh",
        .compression = UEFORMAT_COMPRESSION_NONE,
    };

    auto saved = SaveChecked(ueformat_save_model(&desc, &options), "ueformat_save_model should succeed for a valid model desc");

    auto loaded = ueformat_load_model(saved.Buffer.data, saved.Buffer.size);
    RequireOk(loaded.status, loaded.error, "ueformat_load_model should succeed for bytes produced by save");
    Check(loaded.model != nullptr, "ueformat_load_model should return a non-null model handle");

    auto resaved = SaveChecked(ueformat_model_save(loaded.model, &options), "ueformat_model_save should succeed for a loaded model handle");

    auto reloaded = ueformat_load_model(resaved.Buffer.data, resaved.Buffer.size);
    RequireOk(reloaded.status, reloaded.error, "bytes from ueformat_model_save should load again successfully");

    ueformat_model_free(loaded.model);
    ueformat_model_free(reloaded.model);
}

UEFORMAT_TEST(api, save_load_anim)
{
    const UEFormatVectorKeyDesc positions[] = {
        {0, {1.0f, 0.0f, 0.0f}},
    };
    const UEFormatFloatKeyDesc keys[] = {
        {0, 0.5f},
    };

    UEFormatTrackDesc track{};
    track.bone_name = "pelvis";
    track.position_keys = positions;
    track.position_key_count = 1;

    UEFormatCurveDesc curve{};
    curve.curve_name = "blink";
    curve.keys = keys;
    curve.key_count = 1;

    UEFormatAnimDesc desc{};
    desc.metadata.num_frames = 10;
    desc.metadata.frames_per_second = 30.0f;
    desc.metadata.ref_pose_path = "/Game/Ref";
    desc.tracks = &track;
    desc.track_count = 1;
    desc.curves = &curve;
    desc.curve_count = 1;

    const UEFormatSaveOptions options{
        .object_name = "ApiAnim",
        .compression = UEFORMAT_COMPRESSION_NONE,
    };

    auto saved = SaveChecked(ueformat_save_anim(&desc, &options),"ueformat_save_anim should succeed for a valid anim desc");

    auto loaded = ueformat_load_anim(saved.Buffer.data, saved.Buffer.size);
    RequireOk(loaded.status, loaded.error, "ueformat_load_anim should succeed for bytes produced by save");
    Check(loaded.anim != nullptr, "ueformat_load_anim should return a non-null anim handle");

    SaveChecked(ueformat_anim_save(loaded.anim, &options),"ueformat_anim_save should succeed for a loaded anim handle");
    ueformat_anim_free(loaded.anim);
}

UEFORMAT_TEST(api, save_load_pose)
{
    const UEFormatPoseKeyDesc keys[] = {
        {"jaw", {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
    };

    UEFormatPoseDataDesc poseData{};
    poseData.pose_name = "smile";
    poseData.keys = keys;
    poseData.key_count = 1;

    const char* curveNames[] = {"smile_curve"};

    UEFormatPoseDesc desc{};
    desc.poses = &poseData;
    desc.pose_count = 1;
    desc.curve_names = curveNames;
    desc.curve_name_count = 1;

    const UEFormatSaveOptions options{
        .object_name = "ApiPose",
        .compression = UEFORMAT_COMPRESSION_NONE,
    };

    auto saved = SaveChecked(ueformat_save_pose(&desc, &options), "ueformat_save_pose should succeed for a valid pose desc");

    auto loaded = ueformat_load_pose(saved.Buffer.data, saved.Buffer.size);
    RequireOk(loaded.status, loaded.error, "ueformat_load_pose should succeed for bytes produced by save");
    Check(loaded.pose != nullptr, "ueformat_load_pose should return a non-null pose handle");

    auto resaved = SaveChecked(ueformat_pose_save(loaded.pose, &options), "ueformat_pose_save should succeed for a loaded pose handle");

    auto cppLoad = UEFormat::API::LoadPose(
        std::span<const uint8_t>(resaved.Buffer.data, resaved.Buffer.size));
    Check(cppLoad.Ok(),
        cppLoad.message.empty()
            ? "API::LoadPose should accept bytes from ueformat_pose_save"
            : cppLoad.message);
    Check(cppLoad.value.Poses.size() == 1, "C++ LoadPose should preserve a single pose entry");
    Check(cppLoad.value.Poses[0].PoseName == "smile", "C++ LoadPose should preserve the pose name");
    Check(cppLoad.value.Poses[0].Keys.size() == 1, "C++ LoadPose should preserve pose keys");
    Check(NearlyEqual(cppLoad.value.Poses[0].Keys[0].Location.Y, 1.0f), "C++ LoadPose should preserve pose key location");

    ueformat_pose_free(loaded.pose);
}

UEFORMAT_TEST(api, null_arguments)
{
    const auto badSave = ueformat_save_model(nullptr, nullptr);
    Check(badSave.status == UEFORMAT_INVALID_ARGUMENT, "ueformat_save_model(nullptr, nullptr) should return INVALID_ARGUMENT");

    const auto badLoad = ueformat_load_model(nullptr, 0);
    Check(badLoad.status == UEFORMAT_INVALID_ARGUMENT, "ueformat_load_model(nullptr, 0) should return INVALID_ARGUMENT");
}
