# UEModel Specifications

Binary layout for `.uemodel` files (`Identifier = "UEMODEL"`).

Latest format only. Shared header / [payload sections](generic.md#file-payload) / `FDataAttribute` framing: [generic.md](generic.md).

---

## Sections

Top-level `TArray<FDataAttribute>` after the header:

| Name | Data |
|------|------|
| `LODS` | `TArray<UEModelLOD>` |
| `SKELETON` | `TArray<FDataAttribute>` |
| `COLLISION` | `TArray<FConvexMeshCollision>` |

---

## LOD attributes

Each `UEModelLOD` is:

```text
FString Name                 // e.g. "LOD0"
TArray<FDataAttribute>       // mesh attributes
```

| Name | Data |
|------|------|
| `VERTICES` | `TArray<FVector>` |
| `NORMALS` | `TArray<FNormal>` |
| `TANGENTS` | `TArray<FVector>` |
| `TEXCOORDS` | `TArray<FTexCoordEntry>` |
| `INDICES` | `TArray<uint32>` |
| `VERTEXCOLORS` | `TArray<FVertexColor>` |
| `MATERIALS` | `TArray<FMaterial>` |
| `WEIGHTS` | `TArray<FWeight>` |
| `MORPHTARGETS` | `TArray<FMorphTarget>` |

---

## Skeleton attributes

`SKELETON` body attributes:

| Name | Data |
|------|------|
| `METADATA` | [FSkeletonMetadata](#structures) |
| `BONES` | `TArray<FBone>` |
| `SOCKETS` | `TArray<FSocket>` |
| `VIRTUALBONES` | `TArray<FVirtualBone>` |

---

## Structures

Wire layouts:

```csharp
struct UEModelLOD
{
    FString Name;
    TArray<FDataAttribute> Attributes; // see LOD attributes
}

struct FTexCoordEntry
{
    FString Name;                 // e.g. "UV0", "UV1"
    TArray<FMeshUVFloat> UVs;
}

struct FNormal
{
    float BinormalSign;
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
    int32 FirstIndex;
    int32 NumFaces;
}

struct FWeight
{
    uint16 Bone;
    int32 VertexIndex;
    float Weight;
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
    uint32 VertexIndex;
}

struct FBone
{
    FString BoneName;
    int32 ParentIndex;
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
    TArray<int32> IndexData;      // triangle list
}
```

Logical view of the `SKELETON` attribute bag (not serialized as a flat struct):

```csharp
struct UEModelSkeleton
{
    FSkeletonMetadata Metadata;
    TArray<FBone> Bones;
    TArray<FSocket> Sockets;
    TArray<FVirtualBone> VirtualBones;
}
```
