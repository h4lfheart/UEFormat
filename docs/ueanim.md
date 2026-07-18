# UEAnim Specifications

Binary layout for `.ueanim` files (`Identifier = "UEANIM"`).

Latest format only. Shared header / [payload sections](generic.md#file-payload) / `FDataAttribute` framing: [generic.md](generic.md).

---

## Sections

Top-level `TArray<FDataAttribute>` after the header:

| Name | Data |
|------|------|
| `METADATA` | [FAnimMetadata](#structures) |
| `TRACKS` | `TArray<FTrack>` |
| `CURVES` | `TArray<FCurve>` |

---

## Structures

```csharp
struct FAnimMetadata
{
    int32 NumFrames;
    float FramesPerSecond;
    FString RefPosePath;
    uint8 AdditiveAnimType; // EAdditiveAnimationType
    uint8 RefPoseType; // EAdditiveBasePoseType
    int32 RefFrameIndex;
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
    int32 Frame;
    FVector Value;
}

struct FQuatKey
{
    int32 Frame;
    FQuat Value;
}

struct FFloatKey
{
    int32 Frame;
    float Value;
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
