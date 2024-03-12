#ifndef WGPUFbufferlayout
#define WGPUFbufferlayout

#include <vector>
#include "webgpu/webgpu.h"
#include "Logging.h"
#include "Uncopyable.h"

struct BufferElement
{
	WGPUVertexFormat format;
	uint16_t elementSize = 0;
	uint16_t elementoffset = 0;
	BufferElement(WGPUVertexFormat format, uint16_t elementSize = 0, uint16_t elementoffset = 0)
	{
		this->format = format;
		this->elementSize = elementSize;
		this->elementoffset = elementoffset;
	}
};


class BufferLayout : Uncopyable
{
public:
	inline BufferLayout(bool instanced);


	inline WGPUVertexBufferLayout* getVertexBufferLayout(uint16_t shaderLocOffset = 0)
	{
		int location = 0;
		DestroyAttributes();
		vertexAttributes = new WGPUVertexAttribute[elements.size()];

		for (size_t i = 0; i < elements.size(); i++)
		{
			vertexAttributes[i].format = elements[i].format;
			vertexAttributes[i].shaderLocation = i + shaderLocOffset;
			vertexAttributes[i].offset = elements[i].elementoffset;
		}

		vertexBufferLayout.attributeCount = elements.size();
		vertexBufferLayout.attributes = vertexAttributes;
		vertexBufferLayout.stepMode = mode;
		vertexBufferLayout.arrayStride = stride;
		return &vertexBufferLayout;
	}

	inline void DestroyAttributes()
	{
		delete vertexAttributes;
		vertexAttributes = nullptr;
	}

	template<typename T>
	inline void AddElement(int count) { LOG_ERROR("Unsupported vertex attribute type, skipping"); }

	template<>
	inline void AddElement<int>(int count)
	{
		if (count > 4)
		{
			LOG_ERROR("Unsupported vertex attribute count, skipping");
			return;
		}
		BufferElement element((WGPUVertexFormat)(WGPUVertexFormat_Sint32 + count - 1), sizeof(int) * count, stride);
		elements.push_back(element);
		stride += element.elementSize;
	}

	template<>
	inline void AddElement<float>(int count)
	{
		if (count > 4)
		{
			LOG_ERROR("Unsupported vertex attribute count, skipping");
			return;
		}
		BufferElement element((WGPUVertexFormat)(WGPUVertexFormat_Float32 + count - 1), sizeof(int) * count, stride);
		elements.push_back(element);
		stride += element.elementSize;
	}

	~BufferLayout() { DestroyAttributes(); };

private:
	WGPUVertexBufferLayout vertexBufferLayout{};
	std::vector<BufferElement> elements;
	uint32_t stride = 0;
	WGPUVertexStepMode mode;
	WGPUVertexAttribute* vertexAttributes = nullptr;
};

inline BufferLayout::BufferLayout(bool instanced)
{
	if (!instanced) mode = WGPUVertexStepMode_Vertex;
	else mode = WGPUVertexStepMode_Instance;
}

#endif // WGPUFbufferlayout
