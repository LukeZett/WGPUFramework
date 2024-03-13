#include <filesystem>
#include "glm/glm.hpp"
#include <vector>
#include "Buffer.h"

struct Vertex3D
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

class Object3D
{
public:
	Object3D();
	
	void LoadVertices(std::filesystem::path path);

	~Object3D();

	Buffer& GetVertexBuffer() { return m_buffer.value(); }

	inline uint64_t VertexCount() { return m_vertices.size(); }

private:
	std::optional<Buffer> m_buffer;
	std::vector<Vertex3D> m_vertices;
	
};

