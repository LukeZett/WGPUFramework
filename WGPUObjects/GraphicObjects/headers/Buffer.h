#ifndef WGPUFbuffer
#define WGPUFbuffer


#include "webgpu/webgpu.h"
#include "Device.h"
#include <vector>
#include "BufferLayout.h"
#include "Uncopyable.h"

enum BufferType
{
	Vertex = WGPUBufferUsage_Vertex,
	Storage = WGPUBufferUsage_Storage,
	Uniform = WGPUBufferUsage_Uniform,
	VertexStorage = WGPUBufferUsage_Vertex | WGPUBufferUsage_Storage,
	StorageUniform = WGPUBufferUsage_Storage | WGPUBufferUsage_Uniform,
};

class Buffer : Uncopyable
{
public:
	template<typename T>
	Buffer(const std::vector<T> data, WGPUBufferUsageFlags usage, BufferType type, bool instanced = false);
	Buffer(uint64_t allocationSize, WGPUBufferUsageFlags usage, BufferType type, bool instanced = false);
	~Buffer();

	inline BufferLayout& GetLayout() { return layout; }
	inline WGPUBuffer Get() { return buffer; }
	inline uint64_t Size() { return size; }

	template<typename T>
	inline void Upload(const std::vector<T>& elements, uint32_t startOffset = 0);

	template<typename S>
	inline void Upload(const S* data, uint64_t length, uint64_t startOffset = 0);

private:
	uint64_t size = 0;
	BufferLayout layout;
	WGPUBuffer buffer;
	WGPUBufferDescriptor bufferDesc = {};
};



template<typename T>
inline Buffer::Buffer(const std::vector<T> data, WGPUBufferUsageFlags usage, BufferType type, bool instanced) : layout(instanced)
{
	Buffer(data.size() * sizeof(T), usage, type, instanced);
	Upload(data);
}

template<typename T>
inline void Buffer::Upload(const std::vector<T>& elements, uint32_t startOffset)
{
	size += elements.size() * sizeof(T);
	wgpuQueueWriteBuffer(Device::GetQueue(), buffer, sizeof(T) * (uint64_t)startOffset, &elements[0], elements.size() * sizeof(T));
}

template<typename S>
inline void Buffer::Upload(const S* data, uint64_t length, uint64_t startOffset)
{
	size += length * sizeof(S);
	wgpuQueueWriteBuffer(Device::GetQueue(), buffer, startOffset, data, length * sizeof(S));
}

#endif // !WGPUFbuffer