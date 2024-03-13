#include "Object3D.h"
#define TINYOBJLOADER_IMPLEMENTATION 
#include "tiny_obj_loader.h"
#include <iostream>
#include "Logging.h"

Object3D::Object3D()
{
}

void Object3D::LoadVertices(std::filesystem::path path)
{
    std::string inputfile = path.generic_string();
    tinyobj::ObjReaderConfig reader_config;
    tinyobj::ObjReader reader;


    if (!reader.ParseFromFile(inputfile, reader_config)) {
        if (!reader.Error().empty()) {
            LOG_ERROR("TinyObjReader: " + reader.Error());
        }
    }

    if (!reader.Warning().empty()) {
        LOG_WARN("TinyObjReader: " + reader.Warning());
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {

                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                Vertex3D vertex = Vertex3D(glm::vec3(vx, vy, vz));

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                    vertex.normal = { nx, ny, nz };
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    vertex.uv = { tx, ty };
                }
                m_vertices.push_back(vertex);
            }
            index_offset += fv;
        }
    }

    m_buffer.emplace(sizeof(Vertex3D) * m_vertices.size(), WGPUBufferUsage_CopyDst, Vertex);
    m_buffer.value().Upload(m_vertices, 0);
    m_buffer.value().GetLayout().AddElement<float>(3);
    m_buffer.value().GetLayout().AddElement<float>(3);
    m_buffer.value().GetLayout().AddElement<float>(2);
}





Object3D::~Object3D()
{
}
