#include "AppWindow.h"
#include "Logging.h"
#include "Adapter.h"
#include "Device.h"

AppWindow::AppWindow()
{
}

bool AppWindow::Init(int width, int height, const std::string& name)
{
	if (!glfwInit()) return false;
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
	
	if (!m_window) {
		LOG_ERROR("Window creation failed! ");
		return false;
	}
	else LOG_INFO("GLFW window initialized");
	
	m_width = width;
	m_height = height;
	glfwSetWindowUserPointer(m_window, this);
	glfwSetFramebufferSizeCallback(m_window, onWindowResize);
	return true;
}

void AppWindow::InitSwapChain()
{
	CreateSwapChain();
	}

void AppWindow::OnResize(int width, int height)
{
	m_width = width;
	m_height = height;
	CreateSwapChain();
}

AppWindow::~AppWindow()
{
	if (m_surface) wgpuSurfaceRelease(m_surface);
	if (m_swapChain) wgpuSwapChainRelease(m_swapChain);
	if (m_window) glfwDestroyWindow(m_window);
	glfwTerminate();
}

void AppWindow::CreateSwapChain()
{
	WGPUSwapChainDescriptor chainDesc = {};
	chainDesc.nextInChain = nullptr;
	chainDesc.width = m_width;
	chainDesc.height = m_height;

#ifdef WEBGPU_BACKEND_DAWN
	WGPUTextureFormat chainFormat = WGPUTextureFormat_BGRA8Unorm;
#else
	m_chainFormat = wgpuSurfaceGetPreferredFormat(m_surface, Adapter::Get());
#endif // WEBGPU_BACKEND_DAWN

	chainDesc.format = m_chainFormat;
	chainDesc.usage = WGPUTextureUsage_RenderAttachment;
	chainDesc.presentMode = WGPUPresentMode_Fifo;
	m_swapChain = wgpuDeviceCreateSwapChain(Device::Get(), m_surface, &chainDesc);
	LOG_INFO("Created swap chain " + std::to_string(m_width) + "x" + std::to_string(m_height));
}
