#ifndef WGPUF_BINDGROUP
#define WGPUF_BINDGROUP

#include "webgpu/webgpu.h"
#include "BindGroupLayout.h"
#include "Device.h"
#include <vector>
#include "Buffer.h"
#include "Uncopyable.h"

class BindGroup : Uncopyable
{
public:
	BindGroup();
	
	WGPUBindGroup GetBindGroup(WGPUBindGroupLayout layout);

	void AddUniformBinding(uint16_t bindingIndex, Buffer& uniformBuffer, uint64_t offset, uint64_t uniformSize);

	~BindGroup();

private:
	std::vector<WGPUBindGroupEntry> m_entries;
	WGPUBindGroupDescriptor m_desc{};
	WGPUBindGroup m_bindGroup = nullptr;
};

inline BindGroup::BindGroup()
{
	m_desc.nextInChain = nullptr;
}

inline void BindGroup::AddUniformBinding(uint16_t bindingIndex, Buffer& uniformBuffer, uint64_t offset, uint64_t uniformSize)
{
	m_entries.emplace_back();
	auto& binding = m_entries.back();
	binding.binding = bindingIndex;
	binding.buffer = uniformBuffer.Get();
	binding.offset = offset;
	binding.size = uniformSize;
}

inline WGPUBindGroup BindGroup::GetBindGroup(WGPUBindGroupLayout layout)
{
	if (m_bindGroup != nullptr) wgpuBindGroupRelease(m_bindGroup);
	m_desc.layout = layout;
	m_desc.entryCount = m_entries.size();
	m_desc.entries = &m_entries[0];
	m_bindGroup = wgpuDeviceCreateBindGroup(Device::Get(), &m_desc);
	return m_bindGroup;
}


inline BindGroup::~BindGroup()
{
	if (m_bindGroup != nullptr) wgpuBindGroupRelease(m_bindGroup);
}


#endif // !WGPUF_BINDGROUP