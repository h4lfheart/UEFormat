# UEFormat Specifications

Shared binary conventions used by `.uemodel`, `.ueanim`, and `.uepose`.

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

Used only where the exporter calls `WriteArray`: an inline count followed by that many elements:

```csharp
struct TArray<T>
{
    int32 Count;
    T[Count] Elements;
}
```

This is **not** the same as `FDataChunk.Count`. Most mesh arrays rely on the chunk header count and pack elements with no extra inline size.

### Fixed magic string

`"UEFORMAT"` is **not** an FString. It is exactly **8** UTF-8 bytes with no length prefix.

---

## File Header

```csharp
struct FUEFormatHeader
{
    uint8[8] Magic; // "UEFORMAT"
    FString Identifier; // "UEMODEL" | "UEANIM" | "UEPOSE"
    uint8 FileVersion;
    FString ObjectName;
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

## Chunk Framing

### FDataChunk

Named data chunk with an element count. Unknown chunks should be skipped using `ByteSize`.

```csharp
struct FDataChunk
{
    FString HeaderName;
    int32 Count;
    int32 ByteSize;
    uint8[ByteSize] Data;
}
```

When the chunk holds a packed array, `Count` is the number of elements in `Data` (no nested `TArray` count). Nested members may still use `TArray<T>`.

### FStaticDataChunk

Named data chunk **without** a count (used for LOD bodies):

```csharp
struct FStaticDataChunk
{
    FString HeaderName;
    int32 ByteSize;
    uint8[ByteSize] Data;
}
```

Notes:

- Chunk order is **not guaranteed**. Readers match on `HeaderName`.
- Nested regions (LOD bodies, skeleton bodies) are sequences of chunks inside another chunk's `Data`.

---

## Identifiers & Extensions

| Identifier | Extension | Spec |
|------------|-----------|------|
| `UEMODEL` | `.uemodel` | [uemodel.md](uemodel.md) |
| `UEANIM` | `.ueanim` | [ueanim.md](ueanim.md) |
| `UEPOSE` | `.uepose` | [uepose.md](uepose.md) |