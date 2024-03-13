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

	m_layoutDesc.nextInChain = nullptr;
	m_layoutDesc.bindGroupLayoutCount = 0;
	m_layoutDesc.bindGroupLayouts = nullptr;
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

uint32_t RenderPipeline::AddBindGroupLayout()
{
	BindGroupLayout* groupLayout = new BindGroupLayout();
	m_bindGroups.push_back(new BindGroup());
	m_bindGroupLayouts.push_back(groupLayout);
	return m_bindGroupLayouts.size() - 1;
}

void RenderPipeline::AddUniformToGroup(uint32_t groupID, uint16_t bindingIndex, Buffer& buffer, ShaderAccess access, uint64_t uniformSize, uint64_t offset)
{
	m_bindGroups[groupID]->AddUniformBinding(bindingIndex, buffer, offset, uniformSize);
	m_bindGroupLayouts[groupID]->AddBindingLayout(UniformBuffer, bindingIndex, access, uniformSize);
}

void RenderPipeline::Reload()
{
	if (m_pipeline) wgpuRenderPipelineRelease(m_pipeline);
	if (m_layout) wgpuPipelineLayoutRelease(m_layout);

	m_bindGroupLayoutObjects.clear();
	m_bindGroupObjects.clear();

	for (size_t i = 0; i < m_bindGroupLayouts.size(); i++)
	{
		m_bindGroupLayoutObjects.push_back(m_bindGroupLayouts[i]->GetGroupLayout());
		m_bindGroupObjects.push_back(m_bindGroups[i]->GetBindGroup(m_bindGroupLayoutObjects.back()));
	}
	m_layoutDesc.bindGroupLayoutCount = m_bindGroupLayoutObjects.size();
	
	if (m_bindGroupLayouts.size() > 0)
	{
		m_layoutDesc.bindGroupLayouts = &m_bindGroupLayoutObjects[0];
	}

	m_layout = wgpuDeviceCreatePipelineLayout(Device::Get(), &m_layoutDesc);
	m_desc.layout = m_layout;
	m_pipeline = wgpuDeviceCreateRenderPipeline(Device::Get(), &m_desc);
}

void RenderPipeline::SetBindGroups(WGPURenderPassEncoder encoder)
{
	for (size_t i = 0; i < m_bindGroupObjects.size(); i++)
	{
		wgpuRenderPassEncoderSetBindGroup(encoder, i, m_bindGroupObjects[i], 0, nullptr);
	}
}

RenderPipeline::~RenderPipeline()
{
	for (auto element : m_bindGroupLayouts)
	{
		delete element;
	}
	for (auto element : m_bindGroups)
	{
		delete element;
	}
}
