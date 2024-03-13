#include "WGPUApplication.h"

WGPUApplication::WGPUApplication()
{}


WGPUApplication::~WGPUApplication()
{
	Device::Release();
	Adapter::Release();
	wgpuInstanceRelease(m_wgpuInstance);
}


bool WGPUApplication::Init(const std::string& name, int width, int heigth)
{
	if (!InitWGPU()) return false;
	SetDeviceLimits();
	if (!m_window.Init(width, heigth, name)) return false;
	m_window.InitSurface(m_wgpuInstance);
	if (!Adapter::Init(m_wgpuInstance, m_window.GetSurface())) return false;
	if (!Device::Init(Adapter::Get())) return false;
	m_window.InitSwapChain();
	ConfigurePipeline();
	m_pipeline.Reload();
	return true;
}


bool WGPUApplication::DrawFrame()
{
	if (m_window.ShouldClose()) return false;

	WGPUTextureView nextTexture = m_window.GetNextTexView();
	if (!nextTexture) {
		std::cerr << "Cannot acquire next swap chain texture" << std::endl;
		return false;
	}
	auto renderPassEncoder = m_renderPass.BeginRenderPass(nextTexture);
	wgpuRenderPassEncoderSetPipeline(renderPassEncoder, m_pipeline.Get());
	m_pipeline.SetBindGroups(renderPassEncoder);

	//draw here

	DrawCalls(renderPassEncoder);
	
	m_renderPass.EndRenderPass();
	wgpuTextureViewRelease(nextTexture);
	m_window.PresentFrame();
	m_window.PollEvents();
	Update(1);
	return true;
}

void WGPUApplication::UseDepthTesting()
{
	auto dimensions = m_window.GetWindowSizes();
	m_pipeline.SetDepthTesting(WGPUTextureFormat_Depth24Plus, WGPUCompareFunction_LessEqual, true);
	m_pipeline.SetDepthTexture(WGPUTextureFormat_Depth24Plus, get<0>(dimensions), get<1>(dimensions));
	m_renderPass.UseDepthTesting(m_pipeline.GetDepthTexture().GetView());
}
