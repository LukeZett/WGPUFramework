#include <vector>
#include "webgpu/webgpu.h"
#include "Device.h"

enum ShaderAccess
{
	Vertex = WGPUShaderStage_Vertex,
	Fragment = WGPUShaderStage_Fragment,
	Compute = WGPUShaderStage_Compute,
	None = WGPUShaderStage_None,
};

enum BindingType
{
	UniformBuffer = WGPUBufferBindingType_Uniform,
	Sampler,
	StorageTexture,
	Texture
};


class BindGroupLayout : Uncopyable
{
public:
	BindGroupLayout();

	void AddBindingLayout(BindingType type, uint16_t bindingIndex, ShaderAccess access, uint64_t minBindingSize = 0);

	WGPUBindGroupLayout GetGroupLayout();
	~BindGroupLayout();

private:
	WGPUBindGroupLayout m_layout = nullptr;
	WGPUBindGroupLayoutDescriptor m_desc{};
	std::vector<WGPUBindGroupLayoutEntry> m_entries;

	void setDefault(WGPUBindGroupLayoutEntry& bindingLayout);
};





BindGroupLayout::BindGroupLayout()
{
	m_desc.nextInChain = nullptr;
	m_desc.entryCount = 0;
	m_desc.label = "Base bind group";
}

inline void BindGroupLayout::AddBindingLayout(BindingType type, uint16_t bindingIndex, ShaderAccess access, uint64_t minBindingSize)
{
	m_entries.emplace_back();
	auto& bindingLayout = m_entries.back();
	setDefault(bindingLayout);
	bindingLayout.binding = bindingIndex;

	switch (type)
	{
	case UniformBuffer:
		bindingLayout.buffer.type = WGPUBufferBindingType_Uniform;
		bindingLayout.buffer.minBindingSize = minBindingSize;
		break;
	case Sampler:
		break;
	case StorageTexture:
		break;
	case Texture:
		break;
	default:
		break;
	}

}

inline WGPUBindGroupLayout BindGroupLayout::GetGroupLayout()
{
	if (m_layout != nullptr) wgpuBindGroupLayoutRelease(m_layout);
	
	m_desc.entryCount = m_entries.size();
	m_desc.entries = &m_entries[0];
	m_layout = wgpuDeviceCreateBindGroupLayout(Device::Get(), &m_desc);
	return m_layout;
}

inline BindGroupLayout::~BindGroupLayout()
{
	if (m_layout != nullptr) wgpuBindGroupLayoutRelease(m_layout);
}

inline void BindGroupLayout::setDefault(WGPUBindGroupLayoutEntry& bindingLayout)
{
	bindingLayout.buffer.nextInChain = nullptr;
	bindingLayout.buffer.type = WGPUBufferBindingType_Undefined;
	bindingLayout.buffer.hasDynamicOffset = false;

	bindingLayout.sampler.nextInChain = nullptr;
	bindingLayout.sampler.type = WGPUSamplerBindingType_Undefined;

	bindingLayout.storageTexture.nextInChain = nullptr;
	bindingLayout.storageTexture.access = WGPUStorageTextureAccess_Undefined;
	bindingLayout.storageTexture.format = WGPUTextureFormat_Undefined;
	bindingLayout.storageTexture.viewDimension = WGPUTextureViewDimension_Undefined;

	bindingLayout.texture.nextInChain = nullptr;
	bindingLayout.texture.multisampled = false;
	bindingLayout.texture.sampleType = WGPUTextureSampleType_Undefined;
	bindingLayout.texture.viewDimension = WGPUTextureViewDimension_Undefined;
}
