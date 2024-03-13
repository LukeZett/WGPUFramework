#ifndef WGPUF_RENDERPIPELINE
#define WGPUF_RENDERPIPELINE

#include "webgpu/webgpu.h"
#include "Device.h"
#include "Shader.h"
#include "Uncopyable.h"
#include "Buffer.h"
#include "BindGroup.h"
#include "BindGroupLayout.h"

enum CullMode
{
	None = WGPUCullMode_None,
	Back = WGPUCullMode_Back,
	Front = WGPUCullMode_Front,
};

enum TriangleOrientation
{
	CCW = WGPUFrontFace_CCW,
	CW = WGPUFrontFace_CW,
};

/**
* @brief class representing traditional rasterization pipeline,
* Minimal usage:
* @code{.cpp}
* RenderPipeline pipeline();
* pipeline.SetShaderProgram(shader, VERTEX_MAIN_FUNCTION_NAME, FRAGMENT_MAIN_FUNCTION_NAME); // provide shader program
* pipeline.SetVertexBuffers(0, nullptr); // describe buffer layout
* pipeline.Reload();
* @endcode
*/
class RenderPipeline : Uncopyable
{
public:
	RenderPipeline();
	/**
	* @brief set shader program used by pipeline
	* @param shader - shader class with compiled code
	* @param vertexEntryPoint - name of vertex shader main function
	* @param fragmentEntryPoint - name of fragment shader main function
	*/
	void SetShaderProgram(const Shader& shader, const char* vertexEntryPoint, const char* fragmentEntryPoint);
	/**
	* @brief describe vertex buffers layout
	* @param buffercount - count of vertex buffers
	* @param buffers - pointer to first element of vertex buffer layouts
	*/
	void SetVertexBuffers(uint32_t buffercount, WGPUVertexBufferLayout* buffers);
	/**
	* @brief optional configuration of primitive pipeline state,
	* default is CCW, no culling, triangle list and undefined index format
	*/
	void SetPrimitiveState(TriangleOrientation orientation, CullMode culling, WGPUPrimitiveTopology topology = WGPUPrimitiveTopology_TriangleList, WGPUIndexFormat indexFormat = WGPUIndexFormat_Undefined);
	/**
	* @brief optional configuration of blend factor, default is traditional
	* srcAlpha * RGBSource + (1 - srcAlpha) * RGBDestination
	*/
	void SetRGBBlendState(WGPUBlendFactor sourceFactor, WGPUBlendFactor destFactor, WGPUBlendOperation operation);
	/**
	* @brief optional configuration of blend factor, default is
	* 1 * AlphaDestination
	*/
	void SetAlphaBlendState(WGPUBlendFactor sourceFactor, WGPUBlendFactor destFactor, WGPUBlendOperation operation);

	/**
	* @brief desribe pipeline's render target(s) (max supported is 8)
	*/
	void SetTargets(const WGPUTextureFormat* targetFormats, uint32_t count);

	uint32_t AddBindGroupLayout();

	void AddUniformToGroup(uint32_t groupID, uint16_t bindingIndex, Buffer& buffer, ShaderAccess access, uint64_t uniformSize, uint64_t offset);

	void Reload();

	WGPURenderPipeline Get() { return m_pipeline; }

	void SetBindGroups(WGPURenderPassEncoder encoder);

	~RenderPipeline();

private:
	
	std::vector<BindGroupLayout*> m_bindGroupLayouts;
	std::vector<WGPUBindGroupLayout> m_bindGroupLayoutObjects;

	std::vector<BindGroup*> m_bindGroups;
	std::vector<WGPUBindGroup> m_bindGroupObjects;

	WGPUPipelineLayoutDescriptor m_layoutDesc = {};
	WGPUPipelineLayout m_layout = nullptr;
	
	WGPURenderPipelineDescriptor m_desc = {};
	
	WGPURenderPipeline m_pipeline = nullptr;

	WGPUFragmentState m_fragmentState{};
	WGPUBlendState m_blendState{};
	WGPUColorTargetState m_colorTarget[8];
};







#endif // !WGPUF_RENDERPIPELINE
