#pragma once
#include <DirectXMath.h>

struct CB_VS_vertexshader
{
	DirectX::XMMATRIX mat;
	DirectX::XMMATRIX map[2];
};

struct CB_PS_pixelshader
{
	float alpha = 1.0f;
};

