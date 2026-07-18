# UEModel Specifications

Binary layout for `.uemodel` files (`Identifier = "UEMODEL"`).

Latest format only. Shared header / [attribute sets](generic.md#attribute-sets): [generic.md](generic.md).

---

## Top-level attribute set

After the header, one `FDataAttributeSet`:

| Name | `Data` layout |
|------|---------------|
| `LODS` | `TArray<UEModelLOD>` |
| `SKELETON` | `FDataAttributeSet` |
| `COLLISION` | `TArray<FConvexMeshCollision>` |

---

## `UEModelLOD`

Each element of `LODS`:

```cpp
struct UEModelLOD
{
    FString Name;
    FDataAttributeSet Attributes;
}
```

### LOD attribute set

| Name | `Data` layout |
|------|---------------|
| `VERTICES` | `TArray<FVector>` |
| `NORMALS` | `TArray<FNormal>` |
| `TANGENTS` | `TArray<FVector>` |
| `TEXCOORDS` | `TArray<FTexCoordEntry>` |
| `INDICES` | `TArray<u32>` |
| `VERTEXCOLORS` | `TArray<FVertexColor>` |
| `MATERIALS` | `TArray<FMaterial>` |
| `WEIGHTS` | `TArray<FWeight>` |
| `MORPHTARGETS` | `TArray<FMorphTarget>` |

---

## `SKELETON` attribute set

`Data` of the top-level `SKELETON` attribute is itself an `FDataAttributeSet`:

| Name | `Data` layout |
|------|---------------|
| `METADATA` | `FSkeletonMetadata` |
| `BONES` | `TArray<FBone>` |
| `SOCKETS` | `TArray<FSocket>` |
| `VIRTUALBONES` | `TArray<FVirtualBone>` |

Logical fields (not a flat wire struct):

```cpp
struct UEModelSkeleton
{
    FSkeletonMetadata Metadata;
    TArray<FBone> Bones;
    TArray<FSocket> Sockets;
    TArray<FVirtualBone> VirtualBones;
}
```

---

## Structures

```cpp
struct FTexCoordEntry
{
    FString Name;
    TArray<FMeshUVFloat> UVs;
}

struct FNormal
{
    f32 BinormalSign;
    FVector Normal;
}

struct FSkeletonMetadata
{
    FString Path;
}

struct FVertexColor
{
    FString Name;
    TArray<FColor> Data;
}

struct FMaterial
{
    FString MaterialName;
    FString MaterialPath;
    i32 FirstIndex;
    i32 NumFaces;
}

struct FWeight
{
    u16 Bone;
    i32 VertexIndex;
    f32 Weight;
}

struct FMorphTarget
{
    FString MorphName;
    TArray<FMorphData> MorphData;
}

struct FMorphData
{
    FVector PositionDelta;
    FVector TangentZDelta;
    u32 VertexIndex;
}

struct FBone
{
    FString BoneName;
    i32 ParentIndex;
    FVector Position;
    FQuat Orientation;
}

struct FSocket
{
    FString SocketName;
    FString BoneName;
    FVector RelativeLocation;
    FQuat RelativeRotation;
    FVector RelativeScale;
}

struct FVirtualBone
{
    FString SourceBoneName;
    FString TargetBoneName;
    FString VirtualBoneName;
}

struct FConvexMeshCollision
{
    FString Name;
    TArray<FVector> VertexData;
    TArray<i32> IndexData;
}
```
