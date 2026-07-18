# UEFormat Specifications

Shared binary conventions for `.uemodel`, `.ueanim`, and `.uepose`.

This document describes the **latest** format only (`EUEFormatVersion.LatestVersion` = `AttributeSetFormat` = 10). Older versions are not documented.

All multi-byte integers and floats are **little-endian**. Integers are signed unless noted otherwise.

---

## Primitive Types

| Type | Size | Encoding |
|------|------|----------|
| `bool` | 1 | `0` = false, non-zero = true |
| `u8` | 1 | Unsigned 8-bit |
| `u16` | 2 | Unsigned 16-bit |
| `i32` | 4 | Signed 32-bit |
| `u32` | 4 | Unsigned 32-bit |
| `f32` | 4 | IEEE-754 single |
| `FVector` | 12 | `f32 X, Y, Z` |
| `FQuat` | 16 | `f32 X, Y, Z, W` |
| `FColor` | 4 | `u8 R, G, B, A` |
| `FMeshUVFloat` | 8 | `f32 U, V` |
| `FString` | 4 + N | See below |
| `TArray<T>` | 4 + ... | See below |

### FString

UTF-8 bytes, no null terminator.

```cpp
struct FString
{
    i32 Length;
    u8[Length] Data;
}
```

### TArray\<T\>

Inline count followed by that many elements:

```cpp
struct TArray<T>
{
    i32 Count;
    T[Count] Elements;
}
```

### Fixed magic string

`"UEFORMAT"` is **not** an FString. It is exactly **8** UTF-8 bytes with no length prefix.

---

## File Header

```cpp
struct FUEFormatHeader
{
    u8[8] Magic; // "UEFORMAT"
    FString Identifier; // "UEMODEL" | "UEANIM" | "UEPOSE"
    u8 FileVersion;
    FString ObjectName;
    FString ObjectPath;
    bool IsCompressed;
}
```

### Compression block (only if `IsCompressed == true`)

```cpp
FString CompressionFormat; // "GZIP" | "ZSTD"
i32 UncompressedSize;
i32 CompressedSize;
```

Data bytes follow the header: compressed `CompressedSize` bytes when `IsCompressed`, otherwise the raw remaining file.

---

## Attribute Sets

Every file body (and several nested regions) is an **attribute set**: a counted list of named, length-prefixed payloads. Order is not guaranteed. Empty optional attributes may be omitted.

### Wire layout

```cpp
struct FDataAttribute
{
    FString Name;
    i32 ByteSize;
    u8[ByteSize] Data;
}

struct FDataAttributeSet
{
    i32 Count;
    FDataAttribute[Count] Attributes;
}
```

`ByteSize` is the exact size of `Data` in bytes. It lets readers skip unknown names without understanding the payload.

There is **no** separate element count on `FDataAttribute`. If `Data` holds an array, that array is encoded as a normal `TArray<T>` *inside* `Data`.

### What goes in `Data`

`Name` selects the typed layout. Typical cases:

| `Data` contains | Example names |
|-----------------|---------------|
| A single struct | `METADATA` |
| A `TArray<T>` | `LODS`, `TRACKS`, `VERTICES` |
| Another nested `FDataAttributeSet` | `SKELETON` body; LOD mesh attributes |

Nested attribute sets use the **same** `FDataAttributeSet` framing again inside the parent's `Data`.

### File payload

After the header / decompression, the remainder of the file is **one** top-level `FDataAttributeSet`. Section names for that set are defined per identifier:

- [uemodel.md](uemodel.md)
- [ueanim.md](ueanim.md)
- [uepose.md](uepose.md)

### Worked nest (`.uemodel`)

```text
FUEFormatHeader
FDataAttributeSet                          ← file payload
  └─ "LODS" → TArray<UEModelLOD>
       └─ each UEModelLOD:
            FString Name
            FDataAttributeSet              ← LOD mesh attributes
              ├─ "VERTICES" → TArray<FVector>
              ├─ "INDICES"  → TArray<u32>
              └─ ...
  └─ "SKELETON" → FDataAttributeSet        ← skeleton attributes
       ├─ "METADATA" → FSkeletonMetadata
       ├─ "BONES"    → TArray<FBone>
       └─ ...
  └─ "COLLISION" → TArray<FConvexMeshCollision>
```

Each arrow is the typed contents of that attribute's `Data` field.

---

## Identifiers & Extensions

| Identifier | Extension | Spec |
|------------|-----------|------|
| `UEMODEL` | `.uemodel` | [uemodel.md](uemodel.md) |
| `UEANIM` | `.ueanim` | [ueanim.md](ueanim.md) |
| `UEPOSE` | `.uepose` | [uepose.md](uepose.md) |
