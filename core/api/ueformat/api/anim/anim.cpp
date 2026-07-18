#include "anim.h"

#include <new>

#include "ueformat/api/generic/convert.h"
#include "ueformat/anim/types.h"
#include "ueformat/generic/context.h"
#include "ueformat/generic/header.h"

namespace UEFormat::API
{
    struct AnimHandle
    {
        UEAnim anim;
    };

    UEAnim FromDesc(const UEFormatAnimDesc& desc)
    {
        using namespace Detail;

        UEAnim anim;
        anim.Metadata.NumFrames = desc.metadata.num_frames;
        anim.Metadata.FramesPerSecond = desc.metadata.frames_per_second;
        anim.Metadata.RefPosePath = ToString(desc.metadata.ref_pose_path);
        anim.Metadata.AdditiveAnimType =
            static_cast<EAdditiveAnimationType>(desc.metadata.additive_anim_type);
        anim.Metadata.RefPoseType =
            static_cast<EAdditiveBasePoseType>(desc.metadata.ref_pose_type);
        anim.Metadata.RefFrameIndex = desc.metadata.ref_frame_index;

        if (desc.tracks && desc.track_count > 0)
        {
            anim.Tracks.reserve(static_cast<size_t>(desc.track_count));
            for (int32_t i = 0; i < desc.track_count; ++i)
            {
                const auto& src = desc.tracks[i];
                FTrack track;
                track.BoneName = ToString(src.bone_name);

                if (src.position_keys && src.position_key_count > 0)
                {
                    track.PositionKeys.reserve(static_cast<size_t>(src.position_key_count));
                    for (int32_t k = 0; k < src.position_key_count; ++k)
                    {
                        track.PositionKeys.push_back(FVectorKey{
                            src.position_keys[k].frame,
                            ToVector(src.position_keys[k].value),
                        });
                    }
                }

                if (src.rotation_keys && src.rotation_key_count > 0)
                {
                    track.RotationKeys.reserve(static_cast<size_t>(src.rotation_key_count));
                    for (int32_t k = 0; k < src.rotation_key_count; ++k)
                    {
                        track.RotationKeys.push_back(FQuatKey{
                            src.rotation_keys[k].frame,
                            ToQuat(src.rotation_keys[k].value),
                        });
                    }
                }

                if (src.scale_keys && src.scale_key_count > 0)
                {
                    track.ScaleKeys.reserve(static_cast<size_t>(src.scale_key_count));
                    for (int32_t k = 0; k < src.scale_key_count; ++k)
                    {
                        track.ScaleKeys.push_back(FVectorKey{
                            src.scale_keys[k].frame,
                            ToVector(src.scale_keys[k].value),
                        });
                    }
                }

                anim.Tracks.push_back(std::move(track));
            }
        }

        if (desc.curves && desc.curve_count > 0)
        {
            anim.Curves.reserve(static_cast<size_t>(desc.curve_count));
            for (int32_t i = 0; i < desc.curve_count; ++i)
            {
                const auto& src = desc.curves[i];
                FCurve curve;
                curve.CurveName = ToString(src.curve_name);
                if (src.keys && src.key_count > 0)
                {
                    curve.Keys.reserve(static_cast<size_t>(src.key_count));
                    for (int32_t k = 0; k < src.key_count; ++k)
                    {
                        curve.Keys.push_back(FFloatKey{
                            src.keys[k].frame,
                            src.keys[k].value,
                        });
                    }
                }
                anim.Curves.push_back(std::move(curve));
            }
        }

        return anim;
    }

    Result<ByteBuffer> Save(const UEAnim& anim, const FSaveOptions& options)
    {
        if (options.ObjectName.empty())
        {
            return Result<ByteBuffer>::Failure(Status::InvalidArgument, "object_name is required");
        }

        try
        {
            Context ctx;
            return Result<ByteBuffer>::Success(ctx.Save(anim, options));
        }
        catch (const std::exception& ex)
        {
            return Result<ByteBuffer>::Failure(Status::SerializeError, ex.what());
        }
    }

    Result<ByteBuffer> Save(const UEFormatAnimDesc& desc, const FSaveOptions& options)
    {
        return Save(FromDesc(desc), options);
    }

    Result<UEAnim> LoadAnim(std::span<const u8> data)
    {
        if (data.empty())
        {
            return Result<UEAnim>::Failure(Status::InvalidArgument, "empty buffer");
        }

        try
        {
            Context ctx;
            auto container = ctx.Load(TArray<u8>(data.begin(), data.end()));
            if (container.Header.Identifier != AnimIdentifier)
            {
                return Result<UEAnim>::Failure(
                    Status::TypeMismatch,
                    "expected UEANIM, got '" + container.Header.Identifier + "'");
            }
            return Result<UEAnim>::Success(std::get<UEAnim>(std::move(container.Object)));
        }
        catch (const std::exception& ex)
        {
            return Result<UEAnim>::Failure(Status::SerializeError, ex.what());
        }
    }
}

extern "C" {

UEFORMAT_API UEFormatBufferResult ueformat_save_anim(
    const UEFormatAnimDesc* anim,
    const UEFormatSaveOptions* options)
{
    if (!anim || !options || !options->object_name)
    {
        return UEFormat::API::ToBufferResult(
            UEFormat::API::Result<UEFormat::API::ByteBuffer>::Failure(
                UEFormat::API::Status::InvalidArgument, "ueformat_save_anim: invalid argument"));
    }

    return UEFormat::API::CatchToBuffer([&] {
        return UEFormat::API::Save(*anim, UEFormat::API::ToSaveOptions(*options));
    });
}

UEFORMAT_API UEFormatAnimResult ueformat_load_anim(const uint8_t* data, size_t size)
{
    UEFormatAnimResult out{};
    if (!data || size == 0)
    {
        UEFormat::API::SetThreadError("ueformat_load_anim: invalid argument");
        out.status = UEFORMAT_INVALID_ARGUMENT;
        out.error = UEFormat::API::ThreadError();
        return out;
    }

    auto result = UEFormat::API::LoadAnim(std::span<const uint8_t>(data, size));
    out.status = static_cast<UEFormatStatus>(result.status);
    if (!result.Ok())
    {
        UEFormat::API::SetThreadError(std::move(result.message));
        out.error = UEFormat::API::ThreadError();
        return out;
    }

    auto* handle = new (std::nothrow) UEFormat::API::AnimHandle{std::move(result.value)};
    if (!handle)
    {
        UEFormat::API::SetThreadError("out of memory");
        out.status = UEFORMAT_OUT_OF_MEMORY;
        out.error = UEFormat::API::ThreadError();
        return out;
    }

    out.anim = reinterpret_cast<UEFormatAnim*>(handle);
    return out;
}

UEFORMAT_API void ueformat_anim_free(UEFormatAnim* anim)
{
    delete reinterpret_cast<UEFormat::API::AnimHandle*>(anim);
}

UEFORMAT_API UEFormatBufferResult ueformat_anim_save(
    const UEFormatAnim* anim,
    const UEFormatSaveOptions* options)
{
    if (!anim || !options || !options->object_name)
    {
        return UEFormat::API::ToBufferResult(
            UEFormat::API::Result<UEFormat::API::ByteBuffer>::Failure(
                UEFormat::API::Status::InvalidArgument, "ueformat_anim_save: invalid argument"));
    }

    const auto& handle = *reinterpret_cast<const UEFormat::API::AnimHandle*>(anim);
    return UEFormat::API::CatchToBuffer([&] {
        return UEFormat::API::Save(handle.anim, UEFormat::API::ToSaveOptions(*options));
    });
}

}
