#ifndef WGPUFadapter
#define WGPUFadapter

#include "webgpu/webgpu.h"
#include "Uncopyable.h"

class Adapter : Uncopyable
{
public:
    
    static bool Init(WGPUInstance instance, WGPUSurface surface);
    
    static void Release() { wgpuAdapterRelease(s_instance.m_adapter); }

    static const WGPUAdapter& Get() { return s_instance.m_adapter; }

    ~Adapter() {};
private:
    Adapter() {}
    WGPUAdapter requestAdapter(WGPUInstance instance, WGPURequestAdapterOptions const* options);
    
    static Adapter s_instance;
    WGPUAdapter m_adapter = nullptr;

};


#endif // !WGPUFadapter