#ifndef WGPUFwindow
#define WGPUFwindow


#include "GLFW/glfw3.h"
#include <iostream>
#include "webgpu/webgpu.h"
#include "webgpu/wgpu.h"
#include "glfw3webgpu.h"
#include "Uncopyable.h"

class AppWindow
{
public:
	/**
	* @brief Create unintialized window instance
	*/
	AppWindow();

	/**
	* @brief Initialize window with glfw
	*/
	bool Init(int width, int height, const std::string& name);

	/**
	* @brief returns window's close flag
	*/
	inline bool ShouldClose() { return glfwWindowShouldClose(m_window); }

	/**
	* @brief Process events
	*/
	inline void PollEvents() { return glfwPollEvents(); }

	/**
	* @brief initialize surface for drawing
	* @param instance initialized webgpu instance
	*/
	inline void InitSurface(WGPUInstance instance) { m_surface = glfwGetWGPUSurface(instance, m_window); }

	inline WGPUSurface GetSurface() { return m_surface; }

	/**
	* @brief Fetch texture view from swap chain 
	*/
	inline WGPUTextureView GetNextTexView() { return wgpuSwapChainGetCurrentTextureView(m_swapChain); }

	/**
	* @brief Show frame in window
	*/
	inline void PresentFrame() { wgpuSwapChainPresent(m_swapChain); }

	inline const WGPUTextureFormat& GetTextureFormat() const { return m_chainFormat; }

	/**
	* @brief initialize swap chain
	*/
	void InitSwapChain();

#pragma region eventHandlers
	
	void OnResize(int width, int height);

#pragma endregion

	~AppWindow();

private: // functions
	void CreateSwapChain();

private: // members
	WGPUTextureFormat m_chainFormat = WGPUTextureFormat_BGRA8Unorm;
	GLFWwindow* m_window = nullptr;
	WGPUSurface m_surface = nullptr;
	WGPUSwapChain m_swapChain = nullptr;
	int m_width = 0;
	int m_height = 0;
};

//helper callbacks for GLFW

inline void onWindowResize(GLFWwindow* window, int width, int height)
{
	auto that = reinterpret_cast<AppWindow*>(glfwGetWindowUserPointer(window));
	that->OnResize(width, height);
}

#endif // !WGPUFwindow