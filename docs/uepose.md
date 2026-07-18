# UEPose Specifications

Binary layout for `.uepose` files (`Identifier = "UEPOSE"`).

Latest format only. Shared header / [attribute sets](generic.md#attribute-sets): [generic.md](generic.md).

---

## Top-level attribute set

After the header, one `FDataAttributeSet`:

| Name | `Data` layout |
|------|---------------|
| `POSES` | `TArray<FPoseData>` |
| `CURVES` | `TArray<FString>` |

---

## Structures

```cpp
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
    i32 CurveIndex;
    f32 Influence;
}
```
