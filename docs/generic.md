# UEFormat Specifications

Shared binary conventions for `.uemodel`, `.ueanim`, and `.uepose`.

This document describes the **latest** format only (`EUEFormatVersion.LatestVersion` = `AttributeSetFormat` = 10). Older versions are not documented.

All multi-byte integers and floats are **little-endian**. Integers are signed unless noted otherwise.

---

## Primitive Types

| Type | Size | Encoding |
|------|------|----------|
| `bool` | 1 | `0` = false, non-zero = true |
| `uint8` | 1 | Unsigned byte |
| `uint16` | 2 | Unsigned short |
| `int32` | 4 | Signed int |
| `uint32` | 4 | Unsigned int |
| `float` | 4 | IEEE-754 single |
| `FVector` | 12 | `float X, Y, Z` |
| `FQuat` | 16 | `float X, Y, Z, W` |
| `FColor` | 4 | `uint8 R, G, B, A` |
| `FMeshUVFloat` | 8 | `float U, V` |
| `FString` | 4 + N | See below |
| `TArray<T>` | 4 + ... | See below |

### FString

```csharp
struct FString
{
    int32 Length;
    uint8[Length] Data; // UTF-8, no null terminator
}
```

### TArray\<T\>

Inline count followed by that many elements:

```csharp
struct TArray<T>
{
    int32 Count;
    T[Count] Elements;
}
```

Variable-length payloads inside [`FDataAttribute`](#fdataattribute) `Data` use `TArray`.

### Fixed magic string

`"UEFORMAT"` is **not** an FString. It is exactly **8** UTF-8 bytes with no length prefix.

---

## File Header

```csharp
struct FUEFormatHeader
{
    uint8[8] Magic; // "UEFORMAT"
    FString Identifier; // "UEMODEL" | "UEANIM" | "UEPOSE"
    uint8 FileVersion; // LatestVersion (AttributeSetFormat = 10)
    FString ObjectName;
    FString ObjectPath;
    bool IsCompressed;
}
```

### Compression block (only if `IsCompressed == true`)

```csharp
FString CompressionFormat; // "GZIP" | "ZSTD"
int32 UncompressedSize;
int32 CompressedSize;
```

Data bytes follow the header: compressed `CompressedSize` bytes when `IsCompressed`, otherwise the raw remaining file.

---

## Attribute Framing

### FDataAttribute

Named length-prefixed payload. Layout of `Data` is determined by `Name`. There is **no** per-attribute element count — arrays live inside `Data` as `TArray<T>`.

```csharp
struct FDataAttribute
{
    FString Name;
    int32 ByteSize;
    uint8[ByteSize] Data;
}
```

**Reading**

1. Read `Name` and `ByteSize`.
2. If the name is known, deserialize the typed layout for `Data` directly from the archive (in place).
3. If the name is unknown, `Skip(ByteSize)`.

`ByteSize` exists so unknown attributes can be skipped. Known attributes do not need a sub-archive or EOF loop — the typed payload drives how many bytes are consumed.

### File payload

After the header / decompression, every file payload is:

```text
TArray<FDataAttribute>  // sections for that identifier
```

Wire order:

```text
int32   SectionCount
SectionCount × FDataAttribute
```

Section names and payloads are defined per identifier ([uemodel.md](uemodel.md), [ueanim.md](ueanim.md), [uepose.md](uepose.md)). Section order is **not guaranteed**. Empty optional sections may be omitted.

Section bodies may themselves be containers of `FDataAttribute`s (for example each `UEModelLOD` holds a `TArray<FDataAttribute>` of mesh attributes).

---

## Identifiers & Extensions

| Identifier | Extension | Spec |
|------------|-----------|------|
| `UEMODEL` | `.uemodel` | [uemodel.md](uemodel.md) |
| `UEANIM` | `.ueanim` | [ueanim.md](ueanim.md) |
| `UEPOSE` | `.uepose` | [uepose.md](uepose.md) |
