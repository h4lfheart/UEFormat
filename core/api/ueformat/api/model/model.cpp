#include "model.h"

#include <exception>
#include <new>

#include "ueformat/api/generic/convert.h"
#include "ueformat/generic/context.h"
#include "ueformat/generic/header.h"
#include "ueformat/model/types.h"

namespace UEFormat::API
{
    struct ModelHandle
    {
        UEModel model;
    };

    namespace
    {
        using namespace Detail;

        FNormal ToNormal(const UEFormatNormal& n)
        {
            return FNormal{n.binormal_sign, ToVector(n.normal)};
        }

        UEModelLOD LodFromDesc(const UEFormatModelLodDesc& src)
        {
            UEModelLOD lod;
            lod.Name = ToString(src.name);
            lod.Vertices = MapArray<FVector>(src.vertices, src.vertex_count, ToVector);
            lod.Normals = MapArray<FNormal>(src.normals, src.normal_count, ToNormal);
            lod.Tangents = MapArray<FVector>(src.tangents, src.tangent_count, ToVector);
            lod.Indices = CopyArray(src.indices, src.index_count);

            if (src.texture_coordinates && src.texture_coordinate_count > 0)
            {
                lod.TextureCoordinates.reserve(static_cast<size_t>(src.texture_coordinate_count));
                for (int32_t i = 0; i < src.texture_coordinate_count; ++i)
                {
                    const auto& entry = src.texture_coordinates[i];
                    FTexCoordEntry dst;
                    dst.Name = ToString(entry.name);
                    dst.UVs = MapArray<FMeshUVFloat>(entry.uvs, entry.uv_count, ToUV);
                    lod.TextureCoordinates.push_back(std::move(dst));
                }
            }

            if (src.vertex_colors && src.vertex_color_count > 0)
            {
                lod.VertexColors.reserve(static_cast<size_t>(src.vertex_color_count));
                for (int32_t i = 0; i < src.vertex_color_count; ++i)
                {
                    const auto& entry = src.vertex_colors[i];
                    FVertexColor dst;
                    dst.Name = ToString(entry.name);
                    dst.Data = MapArray<FColor>(entry.data, entry.count, ToColor);
                    lod.VertexColors.push_back(std::move(dst));
                }
            }

            if (src.materials && src.material_count > 0)
            {
                lod.Materials.reserve(static_cast<size_t>(src.material_count));
                for (int32_t i = 0; i < src.material_count; ++i)
                {
                    const auto& entry = src.materials[i];
                    lod.Materials.push_back(FMaterial{
                        ToString(entry.material_name),
                        ToString(entry.material_path),
                        entry.first_index,
                        entry.num_faces,
                    });
                }
            }

            if (src.weights && src.weight_count > 0)
            {
                lod.Weights.reserve(static_cast<size_t>(src.weight_count));
                for (int32_t i = 0; i < src.weight_count; ++i)
                {
                    const auto& entry = src.weights[i];
                    lod.Weights.push_back(FWeight{
                        entry.bone,
                        entry.vertex_index,
                        entry.weight,
                    });
                }
            }

            if (src.morph_targets && src.morph_target_count > 0)
            {
                lod.MorphTargets.reserve(static_cast<size_t>(src.morph_target_count));
                for (int32_t i = 0; i < src.morph_target_count; ++i)
                {
                    const auto& entry = src.morph_targets[i];
                    FMorphTarget dst;
                    dst.MorphName = ToString(entry.morph_name);
                    if (entry.morph_data && entry.morph_data_count > 0)
                    {
                        dst.MorphData.reserve(static_cast<size_t>(entry.morph_data_count));
                        for (int32_t j = 0; j < entry.morph_data_count; ++j)
                        {
                            const auto& md = entry.morph_data[j];
                            dst.MorphData.push_back(FMorphData{
                                ToVector(md.position_delta),
                                ToVector(md.tangent_z_delta),
                                md.vertex_index,
                            });
                        }
                    }
                    lod.MorphTargets.push_back(std::move(dst));
                }
            }

            return lod;
        }

        UEModelSkeleton SkeletonFromDesc(const UEFormatModelSkeletonDesc& src)
        {
            UEModelSkeleton skeleton;
            skeleton.Metadata.Path = ToString(src.metadata_path);

            if (src.bones && src.bone_count > 0)
            {
                skeleton.Bones.reserve(static_cast<size_t>(src.bone_count));
                for (int32_t i = 0; i < src.bone_count; ++i)
                {
                    const auto& bone = src.bones[i];
                    skeleton.Bones.push_back(FBone{
                        ToString(bone.bone_name),
                        bone.parent_index,
                        ToVector(bone.position),
                        ToQuat(bone.orientation),
                    });
                }
            }

            if (src.sockets && src.socket_count > 0)
            {
                skeleton.Sockets.reserve(static_cast<size_t>(src.socket_count));
                for (int32_t i = 0; i < src.socket_count; ++i)
                {
                    const auto& socket = src.sockets[i];
                    skeleton.Sockets.push_back(FSocket{
                        ToString(socket.socket_name),
                        ToString(socket.bone_name),
                        ToVector(socket.relative_location),
                        ToQuat(socket.relative_rotation),
                        ToVector(socket.relative_scale),
                    });
                }
            }

            if (src.virtual_bones && src.virtual_bone_count > 0)
            {
                skeleton.VirtualBones.reserve(static_cast<size_t>(src.virtual_bone_count));
                for (int32_t i = 0; i < src.virtual_bone_count; ++i)
                {
                    const auto& vb = src.virtual_bones[i];
                    skeleton.VirtualBones.push_back(FVirtualBone{
                        ToString(vb.source_bone_name),
                        ToString(vb.target_bone_name),
                        ToString(vb.virtual_bone_name),
                    });
                }
            }

            return skeleton;
        }
    }

    UEModel FromDesc(const UEFormatModelDesc& desc)
    {
        UEModel model;

        if (desc.lods && desc.lod_count > 0)
        {
            model.LODs.reserve(static_cast<size_t>(desc.lod_count));
            for (int32_t i = 0; i < desc.lod_count; ++i)
            {
                model.LODs.push_back(LodFromDesc(desc.lods[i]));
            }
        }

        if (desc.skeleton)
        {
            model.Skeleton = SkeletonFromDesc(*desc.skeleton);
        }

        if (desc.collisions && desc.collision_count > 0)
        {
            model.Collisions.reserve(static_cast<size_t>(desc.collision_count));
            for (int32_t i = 0; i < desc.collision_count; ++i)
            {
                const auto& src = desc.collisions[i];
                FConvexMeshCollision collision;
                collision.Name = ToString(src.name);
                collision.VertexData = Detail::MapArray<FVector>(src.vertex_data, src.vertex_count, ToVector);
                collision.IndexData = CopyArray(src.index_data, src.index_count);
                model.Collisions.push_back(std::move(collision));
            }
        }

        return model;
    }

    Result<ByteBuffer> Save(const UEModel& model, const FSaveOptions& options)
    {
        if (options.ObjectName.empty())
        {
            return Result<ByteBuffer>::Failure(Status::InvalidArgument, "object_name is required");
        }

        try
        {
            Context ctx;
            return Result<ByteBuffer>::Success(ctx.Save(model, options));
        }
        catch (const std::exception& ex)
        {
            return Result<ByteBuffer>::Failure(Status::SerializeError, ex.what());
        }
    }

    Result<ByteBuffer> Save(const UEFormatModelDesc& desc, const FSaveOptions& options)
    {
        return Save(FromDesc(desc), options);
    }

    Result<UEModel> LoadModel(std::span<const u8> data)
    {
        if (data.empty())
        {
            return Result<UEModel>::Failure(Status::InvalidArgument, "empty buffer");
        }

        try
        {
            Context ctx;
            auto container = ctx.Load(TArray<u8>(data.begin(), data.end()));
            if (container.Header.Identifier != ModelIdentifier)
            {
                return Result<UEModel>::Failure(
                    Status::TypeMismatch,
                    "expected UEMODEL, got '" + container.Header.Identifier + "'");
            }
            return Result<UEModel>::Success(std::get<UEModel>(std::move(container.Object)));
        }
        catch (const std::exception& ex)
        {
            return Result<UEModel>::Failure(Status::SerializeError, ex.what());
        }
    }
}

extern "C" {

UEFORMAT_API UEFormatBufferResult ueformat_save_model(
    const UEFormatModelDesc* model,
    const UEFormatSaveOptions* options)
{
    if (!model || !options || !options->object_name)
    {
        return UEFormat::API::ToBufferResult(
            UEFormat::API::Result<UEFormat::API::ByteBuffer>::Failure(
                UEFormat::API::Status::InvalidArgument, "ueformat_save_model: invalid argument"));
    }

    return UEFormat::API::CatchToBuffer([&] {
        return UEFormat::API::Save(*model, UEFormat::API::ToSaveOptions(*options));
    });
}

UEFORMAT_API UEFormatModelResult ueformat_load_model(const uint8_t* data, size_t size)
{
    UEFormatModelResult out{};
    if (!data || size == 0)
    {
        UEFormat::API::SetThreadError("ueformat_load_model: invalid argument");
        out.status = UEFORMAT_INVALID_ARGUMENT;
        out.error = UEFormat::API::ThreadError();
        return out;
    }

    auto result = UEFormat::API::LoadModel(std::span<const uint8_t>(data, size));
    out.status = static_cast<UEFormatStatus>(result.status);
    if (!result.Ok())
    {
        UEFormat::API::SetThreadError(std::move(result.message));
        out.error = UEFormat::API::ThreadError();
        return out;
    }

    auto* handle = new (std::nothrow) UEFormat::API::ModelHandle{std::move(result.value)};
    if (!handle)
    {
        UEFormat::API::SetThreadError("out of memory");
        out.status = UEFORMAT_OUT_OF_MEMORY;
        out.error = UEFormat::API::ThreadError();
        return out;
    }

    out.model = reinterpret_cast<UEFormatModel*>(handle);
    return out;
}

UEFORMAT_API void ueformat_model_free(UEFormatModel* model)
{
    delete reinterpret_cast<UEFormat::API::ModelHandle*>(model);
}

UEFORMAT_API UEFormatBufferResult ueformat_model_save(
    const UEFormatModel* model,
    const UEFormatSaveOptions* options)
{
    if (!model || !options || !options->object_name)
    {
        return UEFormat::API::ToBufferResult(
            UEFormat::API::Result<UEFormat::API::ByteBuffer>::Failure(
                UEFormat::API::Status::InvalidArgument, "ueformat_model_save: invalid argument"));
    }

    const auto& handle = *reinterpret_cast<const UEFormat::API::ModelHandle*>(model);
    return UEFormat::API::CatchToBuffer([&] {
        return UEFormat::API::Save(handle.model, UEFormat::API::ToSaveOptions(*options));
    });
}

}
