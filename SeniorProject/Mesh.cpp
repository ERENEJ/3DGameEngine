#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <locale>
#include <codecvt>

#include "GraphicsEngine.h"
#include "VertexMesh.h"

Mesh::Mesh(const wchar_t* full_path): Resource(full_path)
{
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	//not works after c++17
	//std::string inputfile = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(full_path);

	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &full_path[0], wcslen(full_path), NULL, 0, NULL, NULL);
	std::string inputfile(size_needed, 0);
	//WideCharToMultiByte(CP_UTF8, 0, &full_path[0], wcslen(full_path), &inputfile[0], size_needed, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, &full_path[0], -1, &inputfile[0], size_needed, NULL, NULL);

	//loadObj file path only accpets ascii strings
	bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, inputfile.c_str());

	if (!err.empty()) throw std::exception("Mesh not created successfully");

	if (!res) throw std::exception("Mesh not created successfully");

	if (shapes.size() > 1) throw std::exception("Mesh not created successfully");

	//to hold the vertices and texturecoordinates from tinjobj loader methods
	//for vertex buffer
	std::vector<VertexMesh> list_vertices;
	
	//to hold indices for index buffer
	std::vector<unsigned int> list_indices;
	
	
	//TODO this logic Currently supports only 1 shape
	//iterate over shapes 
	for (size_t s = 0; s < shapes.size(); s++)
	{	
		//every shape must start index 0
		size_t index_offset = 0;

		//to speed up the insertion operation of vector  
		list_vertices.reserve(shapes[s].mesh.indices.size());
		list_indices.reserve(shapes[s].mesh.indices.size());

		//loop through faces of shape
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{	
			// store the face amount of model(a.k.a total triangle count)
			unsigned char num_face_verts = shapes[s].mesh.num_face_vertices[f];


			//each face consists multiple Vertices in this case Triangle
			//that means we need to work on 3 Vertices
			for (unsigned char v = 0; v < num_face_verts; v++)
			{
				tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

				//vertex are stored side by side 
				//Obj1Vertex | Obj1Vertex1 | Obj1Vertex2 | Obj2Vertex0 | Obj2Vertex1 .....
				tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
				tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
				tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 2];

				//Texture coordinate
				// same logic texture coordinates stored side by side
				tinyobj::real_t tx = attribs.texcoords[index.texcoord_index * 2 + 0];
				tinyobj::real_t ty = attribs.texcoords[index.texcoord_index * 2 + 1];

				//Normals for calculating light
				tinyobj::real_t nx = attribs.normals[index.normal_index * 3 + 0];
				tinyobj::real_t ny = attribs.normals[index.normal_index * 3 + 1];
				tinyobj::real_t nz = attribs.normals[index.normal_index * 3 + 2];

				//VertexMesh is customly created just to hold these values
				VertexMesh vertex(Vector3D(vx, vy, vz), Vector2D(tx, ty), Vector3D(nx, ny, nz));
				list_vertices.push_back(vertex);

				//push the indices for index buffer
				list_indices.push_back((unsigned int)index_offset + v);
			}

			index_offset += num_face_verts;
		}
	}

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->getVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);

	m_vertex_buffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&list_vertices[0], sizeof(VertexMesh),
		(UINT)list_vertices.size(), shader_byte_code, (UINT)size_shader);
	m_index_buffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&list_indices[0], (UINT)list_indices.size());

}

Mesh::~Mesh()
{
}

const VertexBufferPtr& Mesh::getVertexBuffer()
{
	return m_vertex_buffer;
}

const IndexBufferPtr& Mesh::getIndexBuffer()
{
	return m_index_buffer;
}