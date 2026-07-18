#pragma once

#include "ueformat/api/generic/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UEFormatTexCoordEntryDesc {
    const char* name;
    const UEFormatMeshUV* uvs;
    int32_t uv_count;
} UEFormatTexCoordEntryDesc;

typedef struct UEFormatVertexColorDesc {
    const char* name;
    const UEFormatColor* data;
    int32_t count;
} UEFormatVertexColorDesc;

typedef struct UEFormatMaterialDesc {
    const char* material_name;
    const char* material_path;
    int32_t first_index;
    int32_t num_faces;
} UEFormatMaterialDesc;

typedef struct UEFormatWeightDesc {
    uint16_t bone;
    int32_t vertex_index;
    float weight;
} UEFormatWeightDesc;

typedef struct UEFormatMorphDataDesc {
    UEFormatVector position_delta;
    UEFormatVector tangent_z_delta;
    uint32_t vertex_index;
} UEFormatMorphDataDesc;

typedef struct UEFormatMorphTargetDesc {
    const char* morph_name;
    const UEFormatMorphDataDesc* morph_data;
    int32_t morph_data_count;
} UEFormatMorphTargetDesc;

typedef struct UEFormatModelLodDesc {
    const char* name;
    const UEFormatVector* vertices;
    int32_t vertex_count;
    const UEFormatNormal* normals;
    int32_t normal_count;
    const UEFormatVector* tangents;
    int32_t tangent_count;
    const UEFormatTexCoordEntryDesc* texture_coordinates;
    int32_t texture_coordinate_count;
    const uint32_t* indices;
    int32_t index_count;
    const UEFormatVertexColorDesc* vertex_colors;
    int32_t vertex_color_count;
    const UEFormatMaterialDesc* materials;
    int32_t material_count;
    const UEFormatWeightDesc* weights;
    int32_t weight_count;
    const UEFormatMorphTargetDesc* morph_targets;
    int32_t morph_target_count;
} UEFormatModelLodDesc;

typedef struct UEFormatBoneDesc {
    const char* bone_name;
    int32_t parent_index;
    UEFormatVector position;
    UEFormatQuat orientation;
} UEFormatBoneDesc;

typedef struct UEFormatSocketDesc {
    const char* socket_name;
    const char* bone_name;
    UEFormatVector relative_location;
    UEFormatQuat relative_rotation;
    UEFormatVector relative_scale;
} UEFormatSocketDesc;

typedef struct UEFormatVirtualBoneDesc {
    const char* source_bone_name;
    const char* target_bone_name;
    const char* virtual_bone_name;
} UEFormatVirtualBoneDesc;

typedef struct UEFormatModelSkeletonDesc {
    const char* metadata_path;
    const UEFormatBoneDesc* bones;
    int32_t bone_count;
    const UEFormatSocketDesc* sockets;
    int32_t socket_count;
    const UEFormatVirtualBoneDesc* virtual_bones;
    int32_t virtual_bone_count;
} UEFormatModelSkeletonDesc;

typedef struct UEFormatConvexCollisionDesc {
    const char* name;
    const UEFormatVector* vertex_data;
    int32_t vertex_count;
    const int32_t* index_data;
    int32_t index_count;
} UEFormatConvexCollisionDesc;

typedef struct UEFormatModelDesc {
    const UEFormatModelLodDesc* lods;
    int32_t lod_count;
    const UEFormatModelSkeletonDesc* skeleton;
    const UEFormatConvexCollisionDesc* collisions;
    int32_t collision_count;
} UEFormatModelDesc;

#ifdef __cplusplus
}
#endif
