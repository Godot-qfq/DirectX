#pragma once
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "AdapterReader.h"
#include "Texture.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/config.h>


class Mesh
{
public:
	Mesh(ID3D11Device * device, ID3D11DeviceContext * deviceContext, std::vector<Vertex> & vertices, std::vector<DWORD> & indices, std::vector<Texture> textures);
	Mesh(const Mesh & mesh);
	void Draw();
	IndexBuffer indexbuffer2;
	VertexBuffer<Vertex> vertexbuffer3;
	std::vector<Texture> textures;
private:
	ID3D11DeviceContext * deviceContext;
};