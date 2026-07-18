#pragma once

#include <exception>
#include <string>
#include <utility>
#include <vector>

#include "types.h"
#include "ueformat/generic/save_options.h"
#include "ueformat/infrastructure/types.h"

namespace UEFormat::API
{
    enum class Status : int32_t
    {
        Ok = UEFORMAT_OK,
        InvalidArgument = UEFORMAT_INVALID_ARGUMENT,
        OutOfMemory = UEFORMAT_OUT_OF_MEMORY,
        SerializeError = UEFORMAT_SERIALIZE_ERROR,
        TypeMismatch = UEFORMAT_TYPE_MISMATCH,
    };

    template <typename T>
    struct Result
    {
        Status status = Status::Ok;
        std::string message;
        T value{};

        bool Ok() const { return status == Status::Ok; }
        explicit operator bool() const { return Ok(); }

        static Result Success(T value)
        {
            Result result;
            result.value = std::move(value);
            return result;
        }

        static Result Failure(Status status, std::string message)
        {
            Result result;
            result.status = status;
            result.message = std::move(message);
            return result;
        }
    };

    using ByteBuffer = std::vector<u8>;

    inline FSaveOptions ToSaveOptions(const UEFormatSaveOptions& options)
    {
        FSaveOptions out;
        out.ObjectName = options.object_name ? options.object_name : "";
        out.ObjectPath = options.object_path ? options.object_path : "";
        out.Compression = static_cast<EFileCompressionFormat>(options.compression);
        out.CompressionLevel = options.compression_level == 0
            ? DefaultCompressionLevel
            : options.compression_level;
        return out;
    }

    inline FSaveOptions ToSaveOptions(const char* objectName, const char* objectPath = "",
        EFileCompressionFormat compression = EFileCompressionFormat::None,
        i32 compressionLevel = DefaultCompressionLevel)
    {
        FSaveOptions out;
        out.ObjectName = objectName ? objectName : "";
        out.ObjectPath = objectPath ? objectPath : "";
        out.Compression = compression;
        out.CompressionLevel = compressionLevel == 0 ? DefaultCompressionLevel : compressionLevel;
        return out;
    }

    UEFormatBufferResult ToBufferResult(Result<ByteBuffer>&& result);
    void SetThreadError(std::string message);
    const char* ThreadError();

    template <typename Fn>
    UEFormatBufferResult CatchToBuffer(Fn&& fn)
    {
        try
        {
            return ToBufferResult(fn());
        }
        catch (const std::exception& ex)
        {
            return ToBufferResult(Result<ByteBuffer>::Failure(Status::SerializeError, ex.what()));
        }
        catch (...)
        {
            return ToBufferResult(Result<ByteBuffer>::Failure(Status::SerializeError, "unknown error"));
        }
    }
}
