#include "Buffer.h"

Buffer::Buffer(uint64_t allocationSize, WGPUBufferUsageFlags usage, BufferType type, bool instanced) : layout(instanced)
{
	bufferDesc.nextInChain = nullptr;
	bufferDesc.label = "base buffer";
	bufferDesc.usage = usage | type;
	bufferDesc.size = allocationSize;
	bufferDesc.mappedAtCreation = false;
	buffer = wgpuDeviceCreateBuffer(Device::Get(), &bufferDesc);
}

Buffer::~Buffer()
{
	wgpuBufferDestroy(buffer);
	wgpuBufferRelease(buffer);
}