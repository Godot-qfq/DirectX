#include "Mesh.h"

Mesh::Mesh(ID3D11Device * device, ID3D11DeviceContext * deviceContext, std::vector<Vertex>& vertices, std::vector<DWORD>& indices, std::vector<Texture> textures)
{
	HRESULT hr;
	this->deviceContext = deviceContext;
	this->textures = textures;
	hr = this->vertexbuffer3.Initialize(device, vertices.data(), vertices.size());
	COM_ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer for mesh.");
	hr = this->indexbuffer2.Initialize(device, indices.data(), indices.size());
	COM_ERROR_IF_FAILED(hr, "Failed to initialize index buffer for mesh.");
}

Mesh::Mesh(const Mesh & mesh)
{
	this->deviceContext = mesh.deviceContext;
	this->indexbuffer2 = mesh.indexbuffer2;
	this->vertexbuffer3 = mesh.vertexbuffer3;
	this->textures = mesh.textures;
}

void Mesh::Draw()
{
	UINT offset = 0;
	for (int i = 0; i < textures.size(); i++)
	{
		if (textures[i].GetType() == aiTextureType::aiTextureType_DIFFUSE)
		{
			this->deviceContext->PSSetShaderResources(0, 1, textures[i].GetTextureResourceViewAddress());
			break;
		}
	}
	this->deviceContext->IASetVertexBuffers(0, 1, this->vertexbuffer3.GetAddressOf(), this->vertexbuffer3.StridePtr(), &offset);
	this->deviceContext->IASetIndexBuffer(this->indexbuffer2.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	this->deviceContext->DrawIndexed(this->indexbuffer2.BufferSize(), 0, 0);
}
