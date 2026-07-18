#include "result.h"

#include <cstring>
#include <new>
#include <string>

namespace
{
    thread_local std::string g_threadError;
}

namespace UEFormat::API
{
    void SetThreadError(std::string message)
    {
        g_threadError = std::move(message);
    }

    const char* ThreadError()
    {
        return g_threadError.c_str();
    }

    UEFormatBufferResult ToBufferResult(Result<ByteBuffer>&& result)
    {
        UEFormatBufferResult out{};
        out.status = static_cast<UEFormatStatus>(result.status);
        out.buffer = {};
        out.error = nullptr;

        if (!result.Ok())
        {
            SetThreadError(std::move(result.message));
            out.error = ThreadError();
            return out;
        }

        if (result.value.empty())
        {
            return out;
        }

        auto* heap = new (std::nothrow) uint8_t[result.value.size()];
        if (!heap)
        {
            SetThreadError("out of memory");
            out.status = UEFORMAT_OUT_OF_MEMORY;
            out.error = ThreadError();
            return out;
        }

        std::memcpy(heap, result.value.data(), result.value.size());
        out.buffer.data = heap;
        out.buffer.size = result.value.size();
        SetThreadError({});
        return out;
    }
}

extern "C" {

UEFORMAT_API void ueformat_buffer_free(UEFormatBuffer* buffer)
{
    if (!buffer)
    {
        return;
    }
    delete[] buffer->data;
    buffer->data = nullptr;
    buffer->size = 0;
}

UEFORMAT_API const char* ueformat_status_string(UEFormatStatus status)
{
    switch (status)
    {
    case UEFORMAT_OK:
        return "ok";
    case UEFORMAT_INVALID_ARGUMENT:
        return "invalid argument";
    case UEFORMAT_OUT_OF_MEMORY:
        return "out of memory";
    case UEFORMAT_SERIALIZE_ERROR:
        return "serialize error";
    case UEFORMAT_TYPE_MISMATCH:
        return "type mismatch";
    default:
        return "unknown status";
    }
}

}
