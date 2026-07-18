#include "pose.h"

#include <new>

#include "ueformat/api/generic/convert.h"
#include "ueformat/generic/context.h"
#include "ueformat/generic/header.h"
#include "ueformat/pose/types.h"

namespace UEFormat::API
{
    struct PoseHandle
    {
        UEPose pose;
    };

    UEPose FromDesc(const UEFormatPoseDesc& desc)
    {
        using namespace Detail;

        UEPose pose;

        if (desc.poses && desc.pose_count > 0)
        {
            pose.Poses.reserve(static_cast<size_t>(desc.pose_count));
            for (int32_t i = 0; i < desc.pose_count; ++i)
            {
                const auto& src = desc.poses[i];
                FPoseData dst;
                dst.PoseName = ToString(src.pose_name);

                if (src.keys && src.key_count > 0)
                {
                    dst.Keys.reserve(static_cast<size_t>(src.key_count));
                    for (int32_t k = 0; k < src.key_count; ++k)
                    {
                        const auto& key = src.keys[k];
                        dst.Keys.push_back(FPoseKey{
                            ToString(key.bone_name),
                            ToVector(key.location),
                            ToQuat(key.rotation),
                            ToVector(key.scale),
                        });
                    }
                }

                if (src.curves && src.curve_count > 0)
                {
                    dst.Curves.reserve(static_cast<size_t>(src.curve_count));
                    for (int32_t c = 0; c < src.curve_count; ++c)
                    {
                        dst.Curves.push_back(FPoseCurveInfluence{
                            src.curves[c].curve_index,
                            src.curves[c].influence,
                        });
                    }
                }

                pose.Poses.push_back(std::move(dst));
            }
        }

        if (desc.curve_names && desc.curve_name_count > 0)
        {
            pose.CurveNames.reserve(static_cast<size_t>(desc.curve_name_count));
            for (int32_t i = 0; i < desc.curve_name_count; ++i)
            {
                pose.CurveNames.push_back(ToString(desc.curve_names[i]));
            }
        }

        return pose;
    }

    Result<ByteBuffer> Save(const UEPose& pose, const FSaveOptions& options)
    {
        if (options.ObjectName.empty())
        {
            return Result<ByteBuffer>::Failure(Status::InvalidArgument, "object_name is required");
        }

        try
        {
            Context ctx;
            return Result<ByteBuffer>::Success(ctx.Save(pose, options));
        }
        catch (const std::exception& ex)
        {
            return Result<ByteBuffer>::Failure(Status::SerializeError, ex.what());
        }
    }

    Result<ByteBuffer> Save(const UEFormatPoseDesc& desc, const FSaveOptions& options)
    {
        return Save(FromDesc(desc), options);
    }

    Result<UEPose> LoadPose(std::span<const u8> data)
    {
        if (data.empty())
        {
            return Result<UEPose>::Failure(Status::InvalidArgument, "empty buffer");
        }

        try
        {
            Context ctx;
            auto container = ctx.Load(TArray<u8>(data.begin(), data.end()));
            if (container.Header.Identifier != PoseIdentifier)
            {
                return Result<UEPose>::Failure(
                    Status::TypeMismatch,
                    "expected UEPOSE, got '" + container.Header.Identifier + "'");
            }
            return Result<UEPose>::Success(std::get<UEPose>(std::move(container.Object)));
        }
        catch (const std::exception& ex)
        {
            return Result<UEPose>::Failure(Status::SerializeError, ex.what());
        }
    }
}

extern "C" {

UEFORMAT_API UEFormatBufferResult ueformat_save_pose(
    const UEFormatPoseDesc* pose,
    const UEFormatSaveOptions* options)
{
    if (!pose || !options || !options->object_name)
    {
        return UEFormat::API::ToBufferResult(
            UEFormat::API::Result<UEFormat::API::ByteBuffer>::Failure(
                UEFormat::API::Status::InvalidArgument, "ueformat_save_pose: invalid argument"));
    }

    return UEFormat::API::CatchToBuffer([&] {
        return UEFormat::API::Save(*pose, UEFormat::API::ToSaveOptions(*options));
    });
}

UEFORMAT_API UEFormatPoseResult ueformat_load_pose(const uint8_t* data, size_t size)
{
    UEFormatPoseResult out{};
    if (!data || size == 0)
    {
        UEFormat::API::SetThreadError("ueformat_load_pose: invalid argument");
        out.status = UEFORMAT_INVALID_ARGUMENT;
        out.error = UEFormat::API::ThreadError();
        return out;
    }

    auto result = UEFormat::API::LoadPose(std::span<const uint8_t>(data, size));
    out.status = static_cast<UEFormatStatus>(result.status);
    if (!result.Ok())
    {
        UEFormat::API::SetThreadError(std::move(result.message));
        out.error = UEFormat::API::ThreadError();
        return out;
    }

    auto* handle = new (std::nothrow) UEFormat::API::PoseHandle{std::move(result.value)};
    if (!handle)
    {
        UEFormat::API::SetThreadError("out of memory");
        out.status = UEFORMAT_OUT_OF_MEMORY;
        out.error = UEFormat::API::ThreadError();
        return out;
    }

    out.pose = reinterpret_cast<UEFormatPose*>(handle);
    return out;
}

UEFORMAT_API void ueformat_pose_free(UEFormatPose* pose)
{
    delete reinterpret_cast<UEFormat::API::PoseHandle*>(pose);
}

UEFORMAT_API UEFormatBufferResult ueformat_pose_save(
    const UEFormatPose* pose,
    const UEFormatSaveOptions* options)
{
    if (!pose || !options || !options->object_name)
    {
        return UEFormat::API::ToBufferResult(
            UEFormat::API::Result<UEFormat::API::ByteBuffer>::Failure(
                UEFormat::API::Status::InvalidArgument, "ueformat_pose_save: invalid argument"));
    }

    const auto& handle = *reinterpret_cast<const UEFormat::API::PoseHandle*>(pose);
    return UEFormat::API::CatchToBuffer([&] {
        return UEFormat::API::Save(handle.pose, UEFormat::API::ToSaveOptions(*options));
    });
}

}
