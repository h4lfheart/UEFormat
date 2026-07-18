# UEPose Specifications

Binary layout for `.uepose` files (`Identifier = "UEPOSE"`).

Latest format only. Shared header / [payload sections](generic.md#file-payload) / `FDataAttribute` framing: [generic.md](generic.md).

---

## Sections

Top-level `TArray<FDataAttribute>` after the header:

| Name | Data |
|------|------|
| `POSES` | `TArray<FPoseData>` |
| `CURVES` | `TArray<FString>` |

---

## Structures

```csharp
struct FPoseData
{
    FString PoseName;
    TArray<FPoseKey> Keys;
    TArray<FPoseCurveInfluence> Curves;
}

struct FPoseKey
{
    FString BoneName;
    FVector Location;
    FQuat Rotation;
    FVector Scale;
}

struct FPoseCurveInfluence
{
    int32 CurveIndex;
    float Influence;
}
```
