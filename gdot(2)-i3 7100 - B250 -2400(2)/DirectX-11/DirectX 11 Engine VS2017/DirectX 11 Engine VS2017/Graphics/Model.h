#pragma once
#include "Mesh.h"
using namespace DirectX;

class Model
{
public:
	bool Initialize(const std::string &filePath,ID3D11Device * device, ID3D11DeviceContext * deviceContext,ConstantBuffer<CB_VS_vertexshader> & cb_vs_vertexshader, ConstantBuffer<CB_PS_pixelshader> & cb_ps_pixelshader, int DIRW);
	bool IncTTg(const std::string &filePath,bool og);
	void Draw(const XMMATRIX & viewProjectionMatrix, float alpha,int DIRW);
	void DRAWCOLL(const XMMATRIX & viewProjectionMatrix, int DIR);
	void rigmk();
	void SetPosition(float x, float y, float z);
	void AdjustPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void AdjustRotation(float x, float y, float z);
	int mp = 0;
	bool g = true;
	XMFLOAT3* sca[25];
	XMFLOAT3* ros2[25];
	XMFLOAT3* pos2[25];
	int tp = 0;
private:
	Assimp::Importer importer;
	std::vector<Mesh> meshes;
	bool LoadModel(const std::string & filePath);
	void ProcessNode(aiNode * node, const aiScene * scene);
	Mesh ProcessMesh(aiMesh * mesh, const aiScene * scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene);
	TextureStorageType DetermineTextureStorageType(const aiScene* pScene, aiMaterial* pMat, unsigned int index, aiTextureType textureType);
	void UpdateWorldMatrix();
	int GetTextureIndex(aiString* pStr);
	ID3D11Device * device = nullptr;
	bool l = false;
	const aiScene* pScene;
	const aiScene* ps[30];
	Assimp::Importer* im[25];
	std::string dir[30];
	ID3D11DeviceContext * deviceContext = nullptr;
	ConstantBuffer<CB_VS_vertexshader> * cb_vs_vertexshader = nullptr;
	ConstantBuffer<CB_PS_pixelshader> * cb_ps_pixelshader =nullptr;
	std::string directory = "";
	std::vector<int> bon;
	XMMATRIX worldMatrix = XMMatrixIdentity();
	XMMATRIX wo = XMMatrixIdentity();
	XMFLOAT3 pos;
	XMFLOAT3 rot;
	int gp = 0;
	std::vector<Texture> diffuseTextures;
};