#ifndef WGPUF_APP
#define WGPUF_APP

#include <string>
#include "Device.h"
#include "Adapter.h"
#include "AppWindow.h"
#include "webgpu/webgpu.h"
#include "RenderPass.h"
#include <vector>
#include "RenderPipeline.h"

/**
* @brief base class for WGPU application, contains window and wgpu device
* @details virtual methods are supposed to be overriden by derived classes,
* they are used for specific operations such as setting required device limits,
* draw calls and configuring render pipeline
*/
class WGPUApplication : Uncopyable
{
public:

	/**
	* @brief Create uninitialized application instance
	*/
	WGPUApplication();

	/**
	* @brief Delete application instance and free all associated
	* resources (glfw context/window, wgpu instance/device/adapter)
	*/
	~WGPUApplication();

	/**
	* @brief Initialize application context and window
	* @param name - window name
	* @param width - window width (will be ignored if running in browser, default 720)
	* @param heigth - window heigth (will be ignored if running in browser, default 480)
	* @returns true if successfull, false otherwise
	*/
	bool Init(const std::string& name, int width = 720, int heigth = 480);

	/**
	* @brief Performs all operations required to draw single frame, calls DrawCalls for drawing operations
	* @returns true if application should continue, false if app should be closed or encountered a failure
	*/
	bool DrawFrame();

	/** @brief Virtual function used to set required device limits by client application */
	virtual void SetDeviceLimits() {};

	/** @brief Virtual function used to configure rendering pipeline by client application */
	virtual void ConfigurePipeline() {};

	/** @brief Virtual function used to perform draw calls in single frame by client application */
	virtual void DrawCalls(WGPURenderPassEncoder& renderEncoder) {};

	/** @brief Virtual function used for updating application state each frame
	* @param deltaTime time since last frame update in milliseconds
	*/
	virtual void Update(float deltaTime) {};

protected:
	std::vector<RenderPipeline*> m_pipelines;
	RenderPipeline m_pipeline;
	AppWindow m_window;
	WGPUInstance m_wgpuInstance = nullptr;
	RenderPass m_renderPass;

private:
	inline bool InitWGPU()
	{
		WGPUInstanceDescriptor desc = {};
		desc.nextInChain = nullptr;
		m_wgpuInstance = wgpuCreateInstance(&desc);
		return m_wgpuInstance != nullptr;
	}
};


#endif // !WGPUF_APP
