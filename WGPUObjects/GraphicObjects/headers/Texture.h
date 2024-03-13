#ifndef WGPUF_TEXTURE
#define WGPUF_TEXTURE

#include "webgpu/webgpu.h"
#include "Device.h"

class Texture
{
public:
	Texture();
	
	void Init(WGPUTextureFormat format, uint32_t width, uint32_t height, WGPUTextureUsage usage);
	
	void CreateView(WGPUTextureFormat viewFormat);

	inline WGPUTexture Get() { return m_texture; };

	inline WGPUTextureView GetView() { return m_view; };

	~Texture();

private:
	WGPUTextureDescriptor m_desc;
	WGPUTextureFormat m_format;
	WGPUTexture m_texture = nullptr;
	WGPUTextureViewDescriptor m_viewDesc;
	WGPUTextureView m_view = nullptr;
};

inline Texture::Texture()
{
	m_desc.nextInChain = nullptr;
	m_desc.dimension = WGPUTextureDimension_2D;
	m_desc.mipLevelCount = 1;
	m_desc.sampleCount = 1;

}

inline void Texture::Init(WGPUTextureFormat format, uint32_t width, uint32_t height, WGPUTextureUsage usage)
{
	if (m_texture != nullptr)
	{
		wgpuTextureDestroy(m_texture);
		wgpuTextureRelease(m_texture);
	}

	m_format = format;
	m_desc.label = "Tex";
	m_desc.format = format;
	m_desc.size = { width, height, 1 };
	m_desc.usage = usage;
	m_desc.viewFormatCount = 1;
	m_desc.viewFormats = &m_format;

	m_texture = wgpuDeviceCreateTexture(Device::Get(), &m_desc);
}

inline void Texture::CreateView(WGPUTextureFormat viewFormat)
{
	if (m_view != nullptr) wgpuTextureViewRelease(m_view);

	m_viewDesc.label = "Tex View";
	m_viewDesc.aspect = WGPUTextureAspect_DepthOnly;
	m_viewDesc.baseArrayLayer = 0;
	m_viewDesc.arrayLayerCount = 1;
	m_viewDesc.baseMipLevel = 0;
	m_viewDesc.mipLevelCount = 1;
	m_viewDesc.dimension = WGPUTextureViewDimension_2D;
	m_viewDesc.format = viewFormat;
	m_viewDesc.nextInChain = nullptr;
	m_view = wgpuTextureCreateView(m_texture, &m_viewDesc);

}

inline Texture::~Texture()
{
	if (m_view != nullptr) wgpuTextureViewRelease(m_view);
	if (m_texture != nullptr)
	{
		wgpuTextureDestroy(m_texture);
		wgpuTextureRelease(m_texture);
	}
}


#endif // !WGPUF_TEXTURE