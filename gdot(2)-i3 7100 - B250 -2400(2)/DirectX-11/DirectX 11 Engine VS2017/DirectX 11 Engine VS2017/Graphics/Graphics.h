#pragma once
#include "AdapterReader.h"
#include "Shaders.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>
#include <io.h>
#include <sys/stat.h>
#include <fstream>
#include "Camera.h"
#include "Model.h"
#include "..\\Timer.h"
#include "ImGUI\\imgui.h"
#include "ImGUI\\imgui_impl_win32.h"
#include "ImGUI\\imgui_impl_dx11.h"

class Graphics
{
public:
	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame();
	Camera camera;
	Model model;
	XMFLOAT3* tt[3];
	int lp = 0;
	void rizemove(int W);
	std::string chCH[100];
private:
	bool InitializeDirectX(HWND hwnd);
	bool InitializeShaders();
	bool InitializeScene();
	std::string cou = "BACK";
	std::string op = "Data\\Objects\\dancing_vampire.dae";//Data\\Objects\\2.fbx,dancing_vampire.dae,platform_pak01_000
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

	VertexShader vertexshader;
	PixelShader pixelshader;
	ConstantBuffer<CB_VS_vertexshader> cb_vs_vertexshader;
	ConstantBuffer<CB_PS_pixelshader> cb_ps_pixelshader;

	void rize();
	void rizemodel();
	Model* a[3];
	bool k = false;
	bool TRV = false;
	float g = 0.0f;
	int TTGGMF = 25;
	int m = 0;
	int switch_on = 0;
	int sw[4];
	int ui = 0;
	int TTGGGG = 0;
	int bxx = 0;
	std::string coug = "State";
	std::string sbkwf = "point";
	std::string stk = "bullet";
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	int windowWidth = 0;
	int windowHeight = 0;
	Timer fpsTimer;
	D3D11_SAMPLER_DESC sampDesc;
};