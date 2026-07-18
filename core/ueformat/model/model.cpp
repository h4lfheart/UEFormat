#include "model.h"

#include "ueformat/general/attribute.h"

namespace UEFormat
{
    FArchive& operator<<(FArchive& archive, UEModelLOD& lod)
    {
        archive << lod.Name;

        IterateDataAttributes(archive, [&](const FDataAttribute& attribute) -> bool
        {
            if (attribute.Name == "VERTICES")
            {
                archive << lod.Vertices;
            }
            else if (attribute.Name == "NORMALS")
            {
                archive << lod.Normals;
            }
            else if (attribute.Name == "TANGENTS")
            {
                archive << lod.Tangents;
            }
            else if (attribute.Name == "TEXCOORDS")
            {
                archive << lod.TextureCoordinates;
            }
            else if (attribute.Name == "INDICES")
            {
                archive << lod.Indices;
            }
            else if (attribute.Name == "VERTEXCOLORS")
            {
                archive << lod.VertexColors;
            }
            else if (attribute.Name == "MATERIALS")
            {
                archive << lod.Materials;
            }
            else if (attribute.Name == "WEIGHTS")
            {
                archive << lod.Weights;
            }
            else if (attribute.Name == "MORPHTARGETS")
            {
                archive << lod.MorphTargets;
            }
            else
            {
                return false;
            }
            return true;
        });

        return archive;
    }

    FArchive& operator<<(FArchive& archive, UEModelSkeleton& skeleton)
    {
        IterateDataAttributes(archive, [&](const FDataAttribute& attribute) -> bool
        {
            if (attribute.Name == "METADATA")
            {
                archive << skeleton.Metadata;
            }
            else if (attribute.Name == "BONES")
            {
                archive << skeleton.Bones;
            }
            else if (attribute.Name == "SOCKETS")
            {
                archive << skeleton.Sockets;
            }
            else if (attribute.Name == "VIRTUALBONES")
            {
                archive << skeleton.VirtualBones;
            }
            else
            {
                return false;
            }

            return true;
        });

        return archive;
    }

    FArchive& operator<<(FArchive& archive, UEModel& model)
    {
        model.LODs.clear();
        model.Skeleton.reset();
        model.Collisions.clear();

        IterateDataAttributes(archive, [&](const FDataAttribute& section) -> bool
        {
            if (section.Name == "LODS")
            {
                archive << model.LODs;
            }
            else if (section.Name == "SKELETON")
            {
                archive << model.Skeleton.emplace();
            }
            else if (section.Name == "COLLISION")
            {
                archive << model.Collisions;
            }
            else
            {
                return false;
            }
            return true;
        });

        return archive;
    }
}
