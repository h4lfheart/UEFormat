#include "archive.h"

#include <cstring>

#include "../infrastructure/exception.h"

namespace UEFormat {

FArchive FArchive::Writer() {
    return FArchive(false, {});
}

FArchive FArchive::Reader(std::span<const u8> data) {
    return FArchive(true, data);
}

FArchive::FArchive(bool loading, std::span<const u8> readData)
    : _loading(loading)
    , _readData(readData) {}

bool FArchive::AtEnd() const {
    return _loading ? _offset >= _readData.size() : false;
}

usize FArchive::TotalSize() const {
    return _loading ? _readData.size() : _writeBuffer.size();
}

usize FArchive::Remaining() const {
    if (!_loading) {
        return 0;
    }
    return _readData.size() - _offset;
}

FArchive& FArchive::Serialize(void* data, usize size) {
    if (_loading) {
        if (_offset + size > _readData.size()) {
            throw UEFormatException("Unexpected end of archive");
        }
        std::memcpy(data, _readData.data() + _offset, size);
        _offset += size;
    } else {
        const auto* bytes = static_cast<const u8*>(data);
        _writeBuffer.insert(_writeBuffer.end(), bytes, bytes + size);
        _offset = _writeBuffer.size();
    }
    return *this;
}

void FArchive::Skip(usize size) {
    if (!_loading) {
        throw UEFormatException("Skip is only valid while loading");
    }
    if (_offset + size > _readData.size()) {
        throw UEFormatException("Skip past end of archive");
    }
    _offset += size;
}

const std::vector<u8>& FArchive::Bytes() const {
    if (_loading) {
        throw UEFormatException("Bytes() is only valid while saving");
    }
    return _writeBuffer;
}

std::vector<u8> FArchive::StealBytes() {
    if (_loading) {
        throw UEFormatException("StealBytes() is only valid while saving");
    }
    _offset = 0;
    return std::move(_writeBuffer);
}

FArchive& FArchive::operator<<(bool& value) {
    u8 byte = value ? 1 : 0;
    Serialize(&byte, sizeof(byte));
    if (_loading) {
        value = byte != 0;
    }
    return *this;
}

FArchive& FArchive::operator<<(u8& value) {
    return Serialize(&value, sizeof(value));
}

FArchive& FArchive::operator<<(u16& value) {
    return Serialize(&value, sizeof(value));
}

FArchive& FArchive::operator<<(i32& value) {
    return Serialize(&value, sizeof(value));
}

FArchive& FArchive::operator<<(u32& value) {
    return Serialize(&value, sizeof(value));
}

FArchive& FArchive::operator<<(f32& value) {
    return Serialize(&value, sizeof(value));
}

FArchive& FArchive::operator<<(FString& value) {
    i32 length = static_cast<i32>(value.size());
    *this << length;
    if (_loading) {
        if (length < 0) {
            throw UEFormatException("Invalid FString length");
        }
        value.resize(static_cast<usize>(length));
    }
    if (length > 0) {
        Serialize(value.data(), static_cast<usize>(length));
    }
    return *this;
}

}
