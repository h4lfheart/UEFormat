<div align="center">

# UEFormat

An intermediate 3D exchange format for Unreal Engine asset extraction.

[![Discord](https://discord.com/api/guilds/866821077769781249/widget.png?style=shield)](https://discord.gg/DZ5YFXdBA6)
[![Blender](https://img.shields.io/badge/Blender-4.2+-blue?logo=blender&logoColor=white&color=orange)](https://www.blender.org/download/)
[![Unreal](https://img.shields.io/badge/Unreal-5.8+-blue?logo=unreal-engine&logoColor=white&color=white)](https://www.unrealengine.com/en-US/download)
[![Release](https://img.shields.io/github/release/h4lfheart/UEFormat)]()
[![Downloads](https://img.shields.io/github/downloads/h4lfheart/UEFormat/total?color=green)]()

</div>

## Plugins
- [Blender](https://github.com/h4lfheart/UEFormat/tree/main/plugins/blender)
- [Unreal Engine](https://github.com/h4lfheart/UEFormat/tree/main/plugins/unreal)

## Supported Exporters
- [FortnitePorting](https://github.com/h4lfheart/FortnitePorting)
- [FModel](https://github.com/4sval/FModel)
- [CUE4Parse](https://github.com/FabianFG/CUE4Parse)

## Format Specification
- [Generic](https://github.com/h4lfheart/UEFormat/tree/main/docs/generic.md)
- [UEModel](https://github.com/h4lfheart/UEFormat/tree/main/docs/uemodel.md)
- [UEAnim](https://github.com/h4lfheart/UEFormat/tree/main/docs/ueanim.md)
- [UEPose](https://github.com/h4lfheart/UEFormat/tree/main/docs/uepose.md)

## Core Library

C++23 library under [`core/`](https://github.com/h4lfheart/UEFormat/tree/main/core) for reading and writing the latest format (`AttributeSetFormat` / v10).

Compression (GZIP / ZSTD) is provided via git submodules under `core/external/` (`zlib`, `zstd`). After cloning, run:

```bash
git submodule update --init --recursive
```

| Target | CMake | Role |
|--------|-------|------|
| `ueformat::ueformat` | `core/lib` | Native C++ types + `Context` load/save |
| `ueformat::ueformat_c` | `core/api` | C FFI for other languages |

```cmake
add_subdirectory(path/to/UEFormat/core)
target_link_libraries(your_target PRIVATE ueformat::ueformat)
target_link_libraries(your_target PRIVATE ueformat::ueformat_c)
```

Link `ueformat::ueformat` for C++, or `ueformat::ueformat_c` for the C FFI.

### C++ usage

```cpp
#include "ueformat/generic/context.h"
#include "ueformat/generic/save_options.h"
#include "ueformat/model/model.h"

using namespace UEFormat;

Context ctx;
UEFormatContainer container = ctx.Load(bytes);
UEModel& model = std::get<UEModel>(container.Object);

FSaveOptions options;
options.ObjectName = "MyMesh";
options.ObjectPath = "/Game/Meshes/MyMesh";
options.Compression = EFileCompressionFormat::ZSTD;

TArray<u8> out = ctx.Save(model, options);
```

Typed helpers also exist under `UEFormat::API` (`LoadModel` / `LoadAnim` / `LoadPose`, plus matching `Save` overloads) if you already know the file type.

### C FFI usage

Stable C entry points for interop (descriptors in, buffers out):

```c
#include "ueformat/api/api.h"

UEFormatSaveOptions options = {
    .object_name = "MyMesh",
    .object_path = "/Game/Meshes/MyMesh",
    .compression = UEFORMAT_COMPRESSION_ZSTD,
    .compression_level = 6, // default level
};

UEFormatBufferResult saved = ueformat_save_model(&desc, &options);
if (saved.status == UEFORMAT_OK)
{
    ueformat_buffer_free(&saved.buffer);
}

UEFormatModelResult loaded = ueformat_load_model(data, size);
if (loaded.status == UEFORMAT_OK)
{
    UEFormatBufferResult again = ueformat_model_save(loaded.model, &options);
    ueformat_buffer_free(&again.buffer);
    ueformat_model_free(loaded.model);
}
```

The same pattern applies to anim (`ueformat_save_anim` / `ueformat_load_anim`) and pose (`ueformat_save_pose` / `ueformat_load_pose`). Always free buffers with `ueformat_buffer_free` and loaded objects with `ueformat_*_free`.
