#include "RenderPipeline.h"
#include "RenderPipeline.h"

RenderPipeline::RenderPipeline()
{
	m_desc.layout = nullptr;
	m_desc.nextInChain = nullptr;
	m_desc.depthStencil = nullptr;
	m_desc.fragment = &m_fragmentState;
	m_fragmentState.targets = m_colorTarget;
	SetRGBBlendState(WGPUBlendFactor_SrcAlpha, WGPUBlendFactor_OneMinusSrcAlpha, WGPUBlendOperation_Add);
	SetAlphaBlendState(WGPUBlendFactor_Zero, WGPUBlendFactor_One, WGPUBlendOperation_Add);
	SetPrimitiveState(TriangleOrientation::CCW, CullMode::None);

	m_desc.multisample.count = 1;
	m_desc.multisample.mask = ~0u;
	m_desc.multisample.alphaToCoverageEnabled = false;
}


void RenderPipeline::SetShaderProgram(const Shader& shader, const char* vertexEntryPoint, const char* fragmentEntryPoint)
{
	m_desc.vertex.module = shader.Get();
	m_desc.vertex.entryPoint = vertexEntryPoint;
	m_desc.vertex.constantCount = 0;
	m_desc.vertex.constants = nullptr;

	m_fragmentState.module = shader.Get();
	m_fragmentState.entryPoint = fragmentEntryPoint;
	m_fragmentState.constantCount = 0;
	m_fragmentState.constants = nullptr;
}


void RenderPipeline::SetVertexBuffers(uint32_t buffercount, WGPUVertexBufferLayout* buffers)
{
	m_desc.vertex.bufferCount = buffercount;
	m_desc.vertex.buffers = buffers;
}


void RenderPipeline::SetPrimitiveState(TriangleOrientation orientation, CullMode culling, WGPUPrimitiveTopology topology, WGPUIndexFormat indexFormat)
{
	m_desc.primitive.frontFace = (WGPUFrontFace)orientation;
	m_desc.primitive.cullMode = (WGPUCullMode)culling;
	m_desc.primitive.stripIndexFormat = indexFormat;
	m_desc.primitive.topology = topology;
}


void RenderPipeline::SetRGBBlendState(WGPUBlendFactor sourceFactor, WGPUBlendFactor destFactor, WGPUBlendOperation operation)
{
	m_blendState.color.srcFactor = sourceFactor;
	m_blendState.color.dstFactor = destFactor;
	m_blendState.color.operation = operation;
}


void RenderPipeline::SetAlphaBlendState(WGPUBlendFactor sourceFactor, WGPUBlendFactor destFactor, WGPUBlendOperation operation)
{
	m_blendState.alpha.srcFactor = sourceFactor;
	m_blendState.alpha.dstFactor = destFactor;
	m_blendState.alpha.operation = operation;
}


void RenderPipeline::SetTargets(const WGPUTextureFormat* targetFormats, uint32_t count)
{
	if (count > 8) return;

	for (size_t i = 0; i < count; i++)
	{
		m_colorTarget[i].format = targetFormats[i];
		m_colorTarget[i].blend = &m_blendState;
		m_colorTarget[i].writeMask = WGPUColorWriteMask_All;
		m_colorTarget[i].nextInChain = nullptr;
	}
	m_fragmentState.targetCount = count;
}

void RenderPipeline::Reload()
{
	if (m_pipeline) wgpuRenderPipelineRelease(m_pipeline);

	m_pipeline = wgpuDeviceCreateRenderPipeline(Device::Get(), &m_desc);
}