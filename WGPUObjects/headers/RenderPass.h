#ifndef WGPUF_RENDERPASS
#define WGPUF_RENDERPASS
#include "webgpu/webgpu.h"
#include "Device.h"
#include "glm/glm.hpp"
#include "Uncopyable.h"

class RenderPass : Uncopyable
{
public:

	inline void SetClearColor(glm::vec4 clearColor) { m_attachments.clearValue = WGPUColor{ clearColor.r, clearColor.g, clearColor.b, clearColor.a }; }

	inline void UseDepthTesting(WGPUTextureView depthView);
	
	inline WGPURenderPassEncoder BeginRenderPass(WGPUTextureView target);
	
	inline void EndRenderPass();

private:
	WGPURenderPassDescriptor m_desc = {};
	WGPURenderPassColorAttachment m_attachments = {};
	WGPURenderPassEncoder m_encoder = nullptr;
	WGPUCommandBufferDescriptor m_commandDesc = {};
	WGPURenderPassDepthStencilAttachment m_depthAttachments = {};

public:
	RenderPass()
	{
		m_desc.nextInChain = nullptr;
		m_desc.colorAttachmentCount = 1;
		m_desc.colorAttachments = &m_attachments;
		m_attachments.resolveTarget = nullptr;
		m_attachments.loadOp = WGPULoadOp_Clear;
		m_attachments.storeOp = WGPUStoreOp_Store;
		m_attachments.clearValue = WGPUColor{ 0.2,0.2,0.2, 1.0 };

		m_desc.depthStencilAttachment = nullptr;
		m_desc.timestampWriteCount = 0;
		m_desc.timestampWrites = nullptr;

		m_commandDesc.nextInChain = nullptr;
		m_commandDesc.label = "Finish RenderPass";
	}
};

inline void RenderPass::UseDepthTesting(WGPUTextureView depthView)
{
	m_depthAttachments.view = depthView;
	m_depthAttachments.depthClearValue = 1.0f;
	m_depthAttachments.depthLoadOp = WGPULoadOp_Clear;
	m_depthAttachments.depthStoreOp = WGPUStoreOp_Store;
	m_depthAttachments.depthReadOnly = false;

	m_depthAttachments.stencilClearValue = 0;
	m_depthAttachments.stencilLoadOp = WGPULoadOp_Clear;
	m_depthAttachments.stencilStoreOp = WGPUStoreOp_Store;

#ifdef WEBGPU_BACKEND_DAWN
	m_depthAttachments.stencilLoadOp = WGPULoadOp_Undefined;
	m_depthAttachments.stencilStoreOp = WGPUStoreOp_Undefined;
	constexpr auto NaNf = std::numeric_limits<float>::quiet_NaN();
	m_depthAttachments.clearDepth = NaNf;
#endif // DAWN

	m_depthAttachments.stencilReadOnly = true;

	m_desc.depthStencilAttachment = &m_depthAttachments;
}

inline WGPURenderPassEncoder RenderPass::BeginRenderPass(WGPUTextureView target)
{
	Device::CreateEncoder();
	m_attachments.view = target;
	m_encoder = wgpuCommandEncoderBeginRenderPass(Device::GetEncoder(), &m_desc);
	
	return m_encoder;
}

inline void RenderPass::EndRenderPass()
{
	wgpuRenderPassEncoderEnd(m_encoder);
	wgpuRenderPassEncoderRelease(m_encoder);
	WGPUCommandBuffer finishCommand = wgpuCommandEncoderFinish(Device::GetEncoder(), &m_commandDesc);

	wgpuQueueSubmit(Device::GetQueue(), 1, &finishCommand);
	wgpuCommandBufferRelease(finishCommand);
	Device::ReleaseEncoder();
}




#endif // !WGPUF_RENDERPASS
