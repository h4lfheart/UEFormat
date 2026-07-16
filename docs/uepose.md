# UEPose Specifications

Binary layout for `.uepose` files (`Identifier = "UEPOSE"`).

---

## Data Chunks

| HeaderName | Count | Data |
|------------|-------|------|
| `POSES` | pose count | [FPoseData](#structures) x `Count` |
| `CURVES` | curve name count | `FString` x `Count` |

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