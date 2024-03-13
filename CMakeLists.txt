cmake_minimum_required(VERSION 3.16)
project(WGPUframework VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(WGPUframework
	"utilities/Logging.cpp"
    "utilities/Logging.h"
    "utilities/Timer.h"
    "Application/AppWindow.h"
    "Application/AppWindow.cpp"
    "Application/WGPUApplication.h"
    "Application/WGPUApplication.cpp"
    "WGPUObjects/headers/Adapter.h"
    "WGPUObjects/src/Adapter.cpp"
    "WGPUObjects/headers/Device.h"
    "WGPUObjects/src/Device.cpp"
    "WGPUObjects/headers/RenderPass.h"
    "WGPUObjects/headers/RenderPipeline.h" 
    "WGPUObjects/GraphicObjects/headers/Shader.h" 
    "WGPUObjects/GraphicObjects/src/Shader.cpp" 
    "WGPUObjects/src/RenderPipeline.cpp" 
    "WGPUObjects/GraphicObjects/headers/Buffer.h" 
    "WGPUObjects/GraphicObjects/src/Buffer.cpp" 
    "WGPUObjects/GraphicObjects/headers/BufferLayout.h" 
    "utilities/Uncopyable.h" 
    "WGPUObjects/headers/BindGroupLayout.h"
    "WGPUObjects/headers/BindGroup.h"
    "Assets/FPSCamera.h"
    "WGPUObjects/GraphicObjects/headers/Texture.h"
    "Assets/tiny_obj_loader.h" "Assets/Object3D.h" "Assets/src/Object3D.cpp")

target_include_directories(WGPUframework PUBLIC utilities)
target_include_directories(WGPUframework PUBLIC Assets)
target_include_directories(WGPUframework PUBLIC Application)
target_include_directories(WGPUframework PUBLIC WGPUObjects/headers)
target_include_directories(WGPUframework PUBLIC WGPUObjects/GraphicObjects/headers)

add_subdirectory(vendor/webgpu)
add_subdirectory(vendor/glfw3webgpu)
add_subdirectory(vendor/glm)
target_link_libraries(WGPUframework PUBLIC webgpu glfw3webgpu)
target_link_libraries(WGPUframework PUBLIC glm)


if (NOT EMSCRIPTEN)
	add_subdirectory(vendor/glfw)
	target_link_libraries(WGPUframework PUBLIC glfw)
endif()

if(MSVC)
    target_compile_options(WGPUframework PRIVATE /W4)
else()
    target_compile_options(WGPUframework PRIVATE -Wall -Wextra -pedantic)
endif()

if(EMSCRIPTEN)
    target_link_options(WGPUframework PRIVATE
        -sUSE_GLFW=3
        -sUSE_WEBGPU
        -sASYNCIFY
        -sALLOW_MEMORY_GROWTH
    )
    set_target_properties(WGPUframework PROPERTIES SUFFIX ".html")
endif()


target_copy_webgpu_binaries(WGPUframework)