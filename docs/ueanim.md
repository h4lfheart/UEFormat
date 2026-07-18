# UEAnim Specifications

Binary layout for `.ueanim` files (`Identifier = "UEANIM"`).

Latest format only. Shared header / [attribute sets](generic.md#attribute-sets): [generic.md](generic.md).

---

## Top-level attribute set

After the header, one `FDataAttributeSet`:

| Name | `Data` layout |
|------|---------------|
| `METADATA` | `FAnimMetadata` |
| `TRACKS` | `TArray<FTrack>` |
| `CURVES` | `TArray<FCurve>` |

---

## Structures

```cpp
struct FAnimMetadata
{
    i32 NumFrames;
    f32 FramesPerSecond;
    FString RefPosePath;
    u8 AdditiveAnimType;
    u8 RefPoseType;
    i32 RefFrameIndex;
}

struct FTrack
{
    FString BoneName;
    TArray<FVectorKey> PositionKeys;
    TArray<FQuatKey> RotationKeys;
    TArray<FVectorKey> ScaleKeys;
}

struct FCurve
{
    FString CurveName;
    TArray<FFloatKey> Keys;
}

struct FVectorKey
{
    i32 Frame;
    FVector Value;
}

struct FQuatKey
{
    i32 Frame;
    FQuat Value;
}

struct FFloatKey
{
    i32 Frame;
    f32 Value;
}
```

### `EAdditiveAnimationType`

| Value | Name |
|------:|------|
| 0 | `AAT_None` |
| 1 | `AAT_LocalSpaceBase` |
| 2 | `AAT_RotationOffsetMeshSpace` |

### `EAdditiveBasePoseType`

| Value | Name |
|------:|------|
| 0 | `ABPT_None` |
| 1 | `ABPT_RefPose` |
| 2 | `ABPT_AnimScaled` |
| 3 | `ABPT_AnimFrame` |
| 4 | `ABPT_LocalAnimFrame` |
