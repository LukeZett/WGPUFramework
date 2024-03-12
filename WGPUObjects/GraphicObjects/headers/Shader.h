#ifndef WGPUF_SHADER
#define WGPUF_SHADER


#include "webgpu/webgpu.h"
#include "Device.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include "Uncopyable.h"

class Shader : Uncopyable
{
public:
	Shader();
	Shader(std::filesystem::path source);
	inline const WGPUShaderModule Get() const { return shaderModule; }

	~Shader();

private:
	std::string LoadFromPath(std::filesystem::path path);
	WGPUShaderModule shaderModule = nullptr;
};

#endif // !WGPUF_SHADER