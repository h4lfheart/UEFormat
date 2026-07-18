#include "compression.h"

#include <limits>
#include <string>

#include <zstd.h>
#include <zlib.h>

#include "../infrastructure/exception.h"
#include "../infrastructure/types.h"

namespace UEFormat
{
    namespace
    {
        TArray<u8> CompressGzip(std::span<const u8> data, i32 level)
        {
            if (level < 0 || level > 9)
            {
                throw UEFormatException("GZIP compression level must be between 0 and 9");
            }

            z_stream stream{};
            const int init = deflateInit2(
                &stream,
                level,
                Z_DEFLATED,
                15 + 16,
                8,
                Z_DEFAULT_STRATEGY);
            if (init != Z_OK)
            {
                throw UEFormatException("Failed to initialize GZIP compressor");
            }

            stream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(data.data()));
            stream.avail_in = static_cast<uInt>(data.size());

            TArray<u8> output;
            output.resize(deflateBound(&stream, static_cast<uLong>(data.size())));

            int status = Z_OK;
            do
            {
                if (stream.total_out >= output.size())
                {
                    output.resize(output.size() * 2 + 64);
                }

                stream.next_out = reinterpret_cast<Bytef*>(output.data() + stream.total_out);
                stream.avail_out = static_cast<uInt>(output.size() - stream.total_out);
                status = deflate(&stream, Z_FINISH);
            }
            while (status == Z_OK);

            deflateEnd(&stream);

            if (status != Z_STREAM_END)
            {
                throw UEFormatException("GZIP compression failed");
            }

            output.resize(stream.total_out);
            return output;
        }

        TArray<u8> DecompressGzip(std::span<const u8> data, i32 uncompressedSize)
        {
            if (uncompressedSize < 0)
            {
                throw UEFormatException("Invalid GZIP uncompressed size");
            }

            TArray<u8> output(static_cast<usize>(uncompressedSize));

            z_stream stream{};
            if (inflateInit2(&stream, 15 + 16) != Z_OK)
            {
                throw UEFormatException("Failed to initialize GZIP decompressor");
            }

            stream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(data.data()));
            stream.avail_in = static_cast<uInt>(data.size());
            stream.next_out = reinterpret_cast<Bytef*>(output.data());
            stream.avail_out = static_cast<uInt>(output.size());

            const int status = inflate(&stream, Z_FINISH);
            inflateEnd(&stream);

            if (status != Z_STREAM_END)
            {
                throw UEFormatException("GZIP decompression failed");
            }

            if (stream.total_out != static_cast<uLong>(uncompressedSize))
            {
                throw UEFormatException("GZIP decompressed size mismatch");
            }

            return output;
        }

        TArray<u8> CompressZstd(std::span<const u8> data, i32 level)
        {
            const size_t bound = ZSTD_compressBound(data.size());
            TArray<u8> output(bound);

            const size_t written = ZSTD_compress(
                output.data(),
                output.size(),
                data.data(),
                data.size(),
                level);

            if (ZSTD_isError(written))
            {
                throw UEFormatException(std::string("ZSTD compression failed: ") + ZSTD_getErrorName(written));
            }

            output.resize(written);
            return output;
        }

        TArray<u8> DecompressZstd(std::span<const u8> data, i32 uncompressedSize)
        {
            if (uncompressedSize < 0)
            {
                throw UEFormatException("Invalid ZSTD uncompressed size");
            }

            TArray<u8> output(static_cast<usize>(uncompressedSize));
            const size_t written = ZSTD_decompress(
                output.data(),
                output.size(),
                data.data(),
                data.size());

            if (ZSTD_isError(written))
            {
                throw UEFormatException(std::string("ZSTD decompression failed: ") + ZSTD_getErrorName(written));
            }

            if (written != static_cast<usize>(uncompressedSize))
            {
                throw UEFormatException("ZSTD decompressed size mismatch");
            }

            return output;
        }
    }

    FString CompressionFormatName(EFileCompressionFormat format)
    {
        switch (format)
        {
        case EFileCompressionFormat::GZIP:
            return "GZIP";
        case EFileCompressionFormat::ZSTD:
            return "ZSTD";
        default:
            return {};
        }
    }

    EFileCompressionFormat CompressionFormatFromName(const FString& name)
    {
        if (name == "GZIP")
        {
            return EFileCompressionFormat::GZIP;
        }
        if (name == "ZSTD")
        {
            return EFileCompressionFormat::ZSTD;
        }
        return EFileCompressionFormat::None;
    }

    TArray<u8> Compress(std::span<const u8> data, EFileCompressionFormat format, i32 level)
    {
        if (data.size() > static_cast<usize>(std::numeric_limits<i32>::max()))
        {
            throw UEFormatException("Data too large to compress");
        }

        if (level == 0)
            level = DefaultCompressionLevel;

        switch (format)
        {
        case EFileCompressionFormat::GZIP:
            return CompressGzip(data, level);
        case EFileCompressionFormat::ZSTD:
            return CompressZstd(data, level);
        default:
            throw UEFormatException("Compression format is None");
        }
    }

    TArray<u8> Decompress(std::span<const u8> data, EFileCompressionFormat format, i32 uncompressedSize)
    {
        switch (format)
        {
        case EFileCompressionFormat::GZIP:
            return DecompressGzip(data, uncompressedSize);
        case EFileCompressionFormat::ZSTD:
            return DecompressZstd(data, uncompressedSize);
        default:
            throw UEFormatException("Unknown compression format");
        }
    }
}
