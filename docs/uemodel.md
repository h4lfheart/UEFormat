# UEModel Specifications

Binary layout for `.uemodel` files (`Identifier = "UEMODEL"`).

---

## Data Chunks

| HeaderName | Count | Data |
|------------|-------|------|
| `LODS` | LOD count | [FStaticDataChunk](generic.md#fstaticdatachunk) per LOD (`LOD0`, `LOD1`, ...) |
| `SKELETON` | `1` | Nested skeleton [FDataChunk](generic.md#fdatachunk)s |
| `COLLISION` | convex count | [FConvexMeshCollision](#structures) x `Count` |

### LOD

Each LOD entry inside `LODS` is an `FStaticDataChunk` named `LOD0`, `LOD1`, etc. Its `Data` is a nested `FDataChunk` stream:

| HeaderName | Count | Packed layout |
|------------|-------|---------------|
| `VERTICES` | vertex count | `FVector` x `Count` |
| `NORMALS` | vertex count | (`float` BinormalSign + `FVector` Normal) x `Count` |
| `TANGENTS` | vertex count | `FVector` x `Count` |
| `TEXCOORDS` | UV channel count | `TArray<FMeshUVFloat>` x `Count` |
| `INDICES` | index count | `uint32` x `Count` (triangle list) |
| `VERTEXCOLORS` | layer count | [FVertexColor](#structures) x `Count` |
| `MATERIALS` | section count | [FMaterial](#structures) x `Count` |
| `WEIGHTS` | influence count | [FWeight](#structures) x `Count` |
| `MORPHTARGETS` | morph count | [FMorphTarget](#structures) x `Count` |

`NORMALS` packing per vertex:

```csharp
float BinormalSign;
FVector Normal;
```

### Skeleton

`SKELETON` data is a nested `FDataChunk` stream:

| HeaderName | Count | Data |
|------------|-------|------|
| `METADATA` | `1` | `FString` skeleton path |
| `BONES` | bone count | [FBone](#structures) x `Count` |
| `SOCKETS` | socket count | [FSocket](#structures) x `Count` |
| `VIRTUALBONES` | virtual bone count | [FVirtualBone](#structures) x `Count` |

---

## Structures

```csharp
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
    uint16 Bone; // bone index
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
    FString BoneName; // parent bone
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
    TArray<int32> IndexData; // triangle list
}
```