#include "device.h"

Device::Device(Buffer *device) : _device(device), _pos(0), _sz(device->size()) {}

size_t Device::read(void *destination, uint64 len) {
    auto sub = _sz-_pos;
    size_t readlen = sub < len ? sub : len;
    memcpy(destination, _device->data() + _pos, readlen);
    _pos += readlen;
    return readlen;
}

void Device::setBuffer(Buffer *device) {
    _device = device;
    _pos = 0;
    _sz = _device->size();
}
