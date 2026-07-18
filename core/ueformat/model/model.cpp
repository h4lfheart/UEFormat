#include "model.h"

#include "ueformat/generic/attribute.h"

namespace UEFormat
{
    FArchive& operator<<(FArchive& archive, UEModelLOD& lod)
    {
        archive << lod.Name;

        FDataAttributeSet attrs;
        attrs.Bind("VERTICES", lod.Vertices);
        attrs.Bind("NORMALS", lod.Normals);
        attrs.Bind("TANGENTS", lod.Tangents);
        attrs.Bind("TEXCOORDS", lod.TextureCoordinates);
        attrs.Bind("INDICES", lod.Indices);
        attrs.Bind("VERTEXCOLORS", lod.VertexColors);
        attrs.Bind("MATERIALS", lod.Materials);
        attrs.Bind("WEIGHTS", lod.Weights);
        attrs.Bind("MORPHTARGETS", lod.MorphTargets);
        archive << attrs;

        return archive;
    }

    FArchive& operator<<(FArchive& archive, UEModelSkeleton& skeleton)
    {
        FDataAttributeSet attrs;
        attrs.Bind("METADATA", skeleton.Metadata);
        attrs.Bind("BONES", skeleton.Bones);
        attrs.Bind("SOCKETS", skeleton.Sockets);
        attrs.Bind("VIRTUALBONES", skeleton.VirtualBones);
        archive << attrs;

        return archive;
    }

    FArchive& operator<<(FArchive& archive, UEModel& model)
    {
        model.LODs.clear();
        model.Skeleton.reset();
        model.Collisions.clear();

        FDataAttributeSet attrs;
        attrs.Bind("LODS", model.LODs);
        attrs.Bind("SKELETON", model.Skeleton);
        attrs.Bind("COLLISION", model.Collisions);
        archive << attrs;

        return archive;
    }
}
