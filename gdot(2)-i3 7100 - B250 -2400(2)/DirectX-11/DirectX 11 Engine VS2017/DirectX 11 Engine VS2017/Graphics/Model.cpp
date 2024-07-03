#include "Model.h"

bool Model::Initialize(const std::string &filePath,ID3D11Device * device, ID3D11DeviceContext * deviceContext,ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader, ConstantBuffer<CB_PS_pixelshader>& cb_ps_pixelshader, int DIRW)
{
	this->device = device;
	this->deviceContext = deviceContext;
	this->cb_vs_vertexshader = &cb_vs_vertexshader;
	this->cb_ps_pixelshader = &cb_ps_pixelshader;
	for (int i = 0; i < DIRW; i++)
	{
		this->pos2[i] = new XMFLOAT3;
		this->ros2[i] = new XMFLOAT3;
		this->sca[i] = new XMFLOAT3;
		this->pos2[i]->x = 0.0f;
		this->pos2[i]->y = 0.0f;
		this->pos2[i]->z = 0.0f;
		this->ros2[i]->x = 0.0f;
		this->ros2[i]->y = 0.0f;
		this->ros2[i]->z = 0.0f;
		this->sca[i]->x = 0.2f;
		this->sca[i]->y = 0.2f;
		this->sca[i]->z = 0.2f;
		im[i] = new Assimp::Importer;
	}
	try
	{
		//Textured Square
		if (!this->LoadModel(filePath))
			return false;
	}
	catch (COMException & exception)
	{
		ErrorLogger::Log(exception);
		return false;
	}
	this->SetPosition(0.0f, 0.0f, 0.0f);
	this->SetRotation(0.0f, 0.0f, 0.0f);
	this->UpdateWorldMatrix();
	return true;
}

bool Model::IncTTg(const std::string & filePath,bool og)
{
	directory = StringConverter::GetDirectoryFromPath(filePath);
	for (int k = 0; k < 30; k++)
	{
		if (dir[k] == filePath)
		{
			this->ProcessNode(ps[k]->mRootNode, ps[k]);
			return true;
		}
	}
	pScene = im[tp]->ReadFile(filePath,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded |
		aiProcess_JoinIdenticalVertices);
	ps[tp]= im[tp +1]->ReadFile(filePath,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded |
		aiProcess_JoinIdenticalVertices);
	dir[tp] = filePath;
	if (og)
	{
		tp++;
	}
	this->ProcessNode(pScene->mRootNode, pScene);
	pScene = nullptr;
	return true;
}

void Model::Draw(const XMMATRIX & viewProjectionMatrix, float alpha,int DIRW)
{
	this->cb_vs_vertexshader->data.mat = this->worldMatrix * viewProjectionMatrix;
	this->cb_vs_vertexshader->data.mat = XMMatrixTranspose(this->cb_vs_vertexshader->data.mat);
	this->cb_vs_vertexshader->ApplyChanges();
	this->cb_ps_pixelshader->data.alpha = alpha;
	this->cb_ps_pixelshader->ApplyChanges();
	this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader->GetAddressOf());
	this->deviceContext->PSSetConstantBuffers(0, 1, this->cb_ps_pixelshader->GetAddressOf());
	if (g)
	{
		for (int i = 0; i < meshes.size(); i++)
		{
			meshes[i].Draw();
		}
	}
	else
	{
		for (int j = 0; j <= DIRW; j++)
		{
			DRAWCOLL(viewProjectionMatrix, j);
		}
	}
}

void Model::DRAWCOLL(const XMMATRIX & viewProjectionMatrix,int DIR)
{
	this->wo = XMMatrixScaling(this->sca[DIR]->x, this->sca[DIR]->y, this->sca[DIR]->z) *XMMatrixRotationRollPitchYaw(this->ros2[DIR]->x, this->ros2[DIR]->y, this->ros2[DIR]->z) * XMMatrixTranslation(pos2[DIR]->x, pos2[DIR]->y, pos2[DIR]->z);
	this->cb_vs_vertexshader->data.mat = this->wo * viewProjectionMatrix; //Calculate World-View-Projection Matrix
	this->cb_vs_vertexshader->data.mat = XMMatrixTranspose(this->cb_vs_vertexshader->data.mat);
	this->wo = XMMatrixScaling(this->sca[DIR]->x, this->sca[DIR]->y, this->sca[DIR]->z) *XMMatrixRotationRollPitchYaw(this->ros2[DIR +1]->x, this->ros2[DIR +1]->y, this->ros2[DIR +1]->z) * XMMatrixTranslation(pos2[DIR +1]->x, pos2[DIR +1]->y, pos2[DIR +1]->z);
	this->cb_vs_vertexshader->data.map[0] = this->wo;
	this->cb_vs_vertexshader->data.map[0] = XMMatrixTranspose(this->cb_vs_vertexshader->data.map[0]);
	this->wo = XMMatrixScaling(this->sca[DIR]->x, this->sca[DIR]->y, this->sca[DIR]->z) *XMMatrixRotationRollPitchYaw(this->ros2[DIR +2]->x, this->ros2[DIR +2]->y, this->ros2[DIR +2]->z) * XMMatrixTranslation(pos2[DIR +2]->x, pos2[DIR +2]->y, pos2[DIR +2]->z);
	this->cb_vs_vertexshader->data.map[1] = this->wo;
	this->cb_vs_vertexshader->data.map[1] = XMMatrixTranspose(this->cb_vs_vertexshader->data.map[1]);
	this->cb_vs_vertexshader->ApplyChanges();
	this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader->GetAddressOf());
	meshes[DIR].Draw();
}

void Model::rigmk()
{
	if (g)
	{
		meshes.erase(meshes.begin() + mp);
	}
	else
	{
		//meshes[mp].textures.clear();
		meshes.erase(meshes.begin() + mp);
	}

}



bool Model::LoadModel(const std::string & filePath)
{
	directory = StringConverter::GetDirectoryFromPath(filePath);
	pScene = importer.ReadFile(filePath,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded|
		aiProcess_JoinIdenticalVertices);
	if (pScene == nullptr)
		return false;
	this->ProcessNode(pScene->mRootNode, pScene);
	pScene = nullptr;
	return true;
}

void Model::ProcessNode(aiNode * node, const aiScene * scene)	
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(this->ProcessMesh(mesh, scene));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene);
	}
}


Mesh Model::ProcessMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;
	vertices.clear();
	indices.clear();
	bon.clear();
	if (mesh->HasBones())
	{
		l = true; 
		aiBone* aib = mesh->mBones[20];
		auto wei = aib->mWeights;
		int numw = aib->mNumWeights;
		for (int j = 0; j < numw; j++)
		{
			int vert = wei[j].mVertexId;
			bon.push_back(vert);
			//OutputDebugStringA(aib->mName.data);
			//OutputDebugStringA(std::to_string(vert).c_str());
			//OutputDebugStringA("cpp");
		}
	}
	else
	{
		l = false;
	}
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertex.texCoord.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.texCoord.y = (float)mesh->mTextureCoords[0][i].y;
		}
		if (l)
		{
			if (i ==bon[gp])
			{
				vertex.bone = 2.0f;
				gp++;
			}
			else
			{
				vertex.bone = 1.0f;
			}
		}
		else
		{
			vertex.bone = 90.0f;
		}
		vertices.push_back(vertex);
	}

	//Get indices
	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	std::vector<Texture> textures;
	textures.clear();
	diffuseTextures.clear();
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	diffuseTextures = LoadMaterialTextures(material, aiTextureType::aiTextureType_DIFFUSE, scene);
	textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());
	gp = 0;
	return Mesh(this->device, this->deviceContext, vertices, indices, textures);
}
TextureStorageType Model::DetermineTextureStorageType(const aiScene * pScene, aiMaterial * pMat, unsigned int index, aiTextureType textureType)
{
	if (pMat->GetTextureCount(textureType) == 0)
		return TextureStorageType::None;

	aiString path;
	pMat->GetTexture(textureType, index, &path);
	std::string texturePath = path.C_Str();
	//Check if texture is an embedded indexed texture by seeing if the file path is an index #
	if (texturePath[0] == '*')
	{
		if (pScene->mTextures[0]->mHeight == 0)
		{
			return TextureStorageType::EmbeddedIndexCompressed;
		}
		else
		{
			assert("SUPPORT DOES NOT EXIST YET FOR INDEXED NON COMPRESSED TEXTURES!" && 0);
			return TextureStorageType::EmbeddedIndexNonCompressed;
		}
	}
	//Check if texture is an embedded texture but not indexed (path will be the texture's name instead of #)
	if (auto pTex = pScene->GetEmbeddedTexture(texturePath.c_str()))
	{
		if (pTex->mHeight == 0)
		{
			return TextureStorageType::EmbeddedCompressed;
		}
		else
		{
			assert("SUPPORT DOES NOT EXIST YET FOR EMBEDDED NON COMPRESSED TEXTURES!" && 0);
			return TextureStorageType::EmbeddedNonCompressed;
		}
	}
	//Lastly check if texture is a filepath by checking for period before extension name
	if (texturePath.find('.') != std::string::npos)
	{
		return TextureStorageType::Disk;
	}

	return TextureStorageType::None; // No texture exists
}
std::vector<Texture> Model::LoadMaterialTextures(aiMaterial * pMaterial, aiTextureType textureType, const aiScene * pScene)
{
	std::vector<Texture> materialTextures;
	TextureStorageType storetype = TextureStorageType::Invalid;
	unsigned int textureCount = pMaterial->GetTextureCount(textureType);

	if (textureCount == 0) //If there are no textures
	{
		storetype = TextureStorageType::None;
		aiColor3D aiColor(0.0f, 0.0f, 0.0f);
		switch (textureType)
		{
		case aiTextureType_DIFFUSE:
			pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
			if (aiColor.IsBlack()) //If color = black, just use grey
			{
				materialTextures.push_back(Texture(this->device, Colors::UnloadedTextureColor, textureType));
				return materialTextures;
			}
			materialTextures.push_back(Texture(this->device, Color(aiColor.r * 299, aiColor.g * 299, aiColor.b * 123), textureType));
			return materialTextures;
		}
	}
	else
	{
		for (UINT i = 0; i < textureCount; i++)
		{
			aiString path;
			pMaterial->GetTexture(textureType, i, &path);
			TextureStorageType storetype = DetermineTextureStorageType(pScene, pMaterial, i, textureType);
			switch (storetype)
			{
			case TextureStorageType::EmbeddedIndexCompressed:
			{
				int index = GetTextureIndex(&path);
				Texture embeddedIndexedTexture(this->device,
					reinterpret_cast<uint8_t*>(pScene->mTextures[index]->pcData),
					pScene->mTextures[index]->mWidth,
					textureType);
				materialTextures.push_back(embeddedIndexedTexture);
				break;
			}
			case TextureStorageType::EmbeddedCompressed:
			{
				const aiTexture * pTexture = pScene->GetEmbeddedTexture(path.C_Str());
				Texture embeddedTexture(this->device,
					reinterpret_cast<uint8_t*>(pTexture->pcData),
					pTexture->mWidth,
					textureType);
				materialTextures.push_back(embeddedTexture);
				break;
			}
			case TextureStorageType::Disk:
			{
				std::string filename = this->directory + '\\' + path.C_Str();
				Texture diskTexture(this->device, filename, textureType);
				materialTextures.push_back(diskTexture);
				break;
			}
			}
		}
	}
	if (materialTextures.size() == 0)
	{
		materialTextures.push_back(Texture(this->device, Colors::UnhandledTextureColor, aiTextureType::aiTextureType_DIFFUSE));
	}
	return materialTextures;
}


void Model::SetPosition(float x, float y, float z)
{
	this->pos = XMFLOAT3(x, y, z);
	//this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}


void Model::AdjustPosition(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	//this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}


void Model::SetRotation(float x, float y, float z)
{
	this->rot = XMFLOAT3(x, y, z);
	//this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}


void Model::AdjustRotation(float x, float y, float z)
{
	this->rot.x += x;
	this->rot.y += y;
	this->rot.z += z;
	//this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}


void Model::UpdateWorldMatrix()
{
	this->worldMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) *XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z) * XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
}

int Model::GetTextureIndex(aiString * pStr)
{
	assert(pStr->length >= 2);
	return atoi(&pStr->C_Str()[1]);
}
