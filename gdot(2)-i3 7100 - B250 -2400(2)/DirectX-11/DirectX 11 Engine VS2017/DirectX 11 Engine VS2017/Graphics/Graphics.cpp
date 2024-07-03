#include "Graphics.h"
char texbox[55] = {"Data\\Objects\\dancing_vampire.dae"};


bool Graphics::Initialize(HWND hwnd, int width, int height)
{
	this->windowWidth = width;
	this->windowHeight = height;
	this->fpsTimer.Start();
	sw[0] = 90;
	sw[1] = 0;
	sw[2] = 0;
	sw[3] = 0;
	if (!InitializeDirectX(hwnd))
		return false;

	if (!InitializeShaders())
		return false;

	if (!InitializeScene())
		return false;

	//Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(this->device.Get(), this->deviceContext.Get());
	//ImGui::StyleColorsLight();
	ImGui::StyleColorsClassic();
	return true;
}

void Graphics::RenderFrame()
{
	float bgcolor[] = { 0.7f, 0.3f, 0.3f, 1.0f };
	this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgcolor);
	this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	this->deviceContext->IASetInputLayout(this->vertexshader.GetInputLayout());
	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	this->deviceContext->RSSetState(this->rasterizerState.Get());
	this->deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
	this->deviceContext->OMSetBlendState(this->blendState.Get(), NULL, 0xFFFFFFFF);//this->blendState.Get()
	this->deviceContext->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());
	this->deviceContext->VSSetShader(vertexshader.GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(pixelshader.GetShader(), NULL, 0);
	static float alpha = 0.5f;
	{
		this->model.Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix(), alpha,sw[0]);
		switch (switch_on)
		{
		case 1:
			rizemove(0);
			this->a[0]->Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix(), alpha,sw[1]);
			break;
		case 2:
			rizemove(0);
			this->a[0]->Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix(), alpha,sw[1]);
			rizemove(1);
			this->a[1]->Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix(), alpha,sw[2]);
			break;
		case 3:
			rizemove(0);
			this->a[0]->Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix(), alpha, sw[1]);
			rizemove(1);
			this->a[1]->Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix(), alpha, sw[2]);
			rizemove(2);
			this->a[2]->Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix(), alpha, sw[3]);
		default:
			break;
		}
	}
	static int fpsCounter = 0;
	
	static std::string fpsString = "FPS: 0";
	fpsCounter += 1;
	if (fpsTimer.GetMilisecondsElapsed() > 1000.0)//1000.0
	{
		fpsString = "FPS: " + std::to_string(fpsCounter);
		fpsCounter = 0;
		fpsTimer.Restart();
	}
	spriteBatch->Begin();
	spriteFont->DrawString(spriteBatch.get(), StringConverter::StringToWide(fpsString).c_str(), DirectX::XMFLOAT2(0, 0), DirectX::Colors::Gold, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
	spriteBatch->End();
	static int counter = 0;
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Object");
	//stk = "time: " + std::to_string(timkk.GetMilisecondsElapsed() /100000);
	std::string co = "model: " + cou;
	if (ImGui::Button("Add Object"))
	{
		if (TRV)
		{
			model.IncTTg(texbox,false);
		}
		else
		{
			const char* f = texbox;
			FILE* fl = fopen(f, "rb");
			int size = filelength(fileno(fl));
			fclose(fl);
			bxx += size;
			if (bxx > 31000000)
			{
				bxx = 0;
				a[ui]->IncTTg(texbox,true);//nanosuit\\nanosuit.obj,Data\\Objects\\2.fbx
			}
			else
			{
				a[ui]->IncTTg(texbox,false);//nanosuit\\nanosuit.obj,Data\\Objects\\2.fbx
			}
			//OutputDebugStringA(std::to_string(size).c_str());
		}
	}
	if (ImGui::Button("Create Object"))
	{
		this->rizemodel();
	}
	ImGui::DragInt("Main body", &ui, 1, 0, 3);
	ImGui::DragInt("child body", &lp, 1, 0, TTGGMF);
	ImGui::DragInt("Rendering Quantity", &sw[lp], 1, 0, TTGGMF);
	if (ImGui::Button("Delete body"))
	{
		if (TRV)
		{
			model.rigmk();
		}
		else
		{
			a[ui]->rigmk();
		}
	}
	if (ImGui::BeginCombo("Status Change", coug.c_str()))
	{
		if (ImGui::Selectable("Delete scenes or body"))
		{
			if (TRV)
			{
				TRV = false;
			}
			else
			{
				TRV = true;
			}
		}
		if (ImGui::Selectable("All or Alone Rendering"))
		{
			if (a[ui]->g)
			{
				a[ui]->g = false;
			}
			else
			{
				a[ui]->g = true;
			}
		}
		ImGui::EndCombo();
	}
	//ImGui::Text(std::to_string(ui).c_str());
	//ImGui::Text(stk.c_str());
	//std::string clickCount = "Click Count: " + std::to_string(counter);
	//ImGui::Text(clickCount.c_str());
	ImGui::DragFloat("Translation X", &tt[ui]->x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat("Translation Y", &tt[ui]->y, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat("Translation Z", &tt[ui]->z, 0.1f, -100.0f, 100.0f);
	if (TTGGGG >0)
	{
		ImGui::DragFloat("scax", &a[ui]->sca[lp]->x, 0.01f, 0.0f, 10.0f);
		ImGui::DragFloat("scay", &a[ui]->sca[lp]->y, 0.01f, 0.0f, 10.0f);
		ImGui::DragFloat("scaz", &a[ui]->sca[lp]->z, 0.01f, 0.0f, 10.0f);
		ImGui::DragFloat("roex", &a[ui]->ros2[lp]->x, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat("roey", &a[ui]->ros2[lp]->y, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat("roeZ", &a[ui]->ros2[lp]->z, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat("posx", &a[ui]->pos2[lp]->x, 0.1f, -100.0f, 100.0f);
		ImGui::DragFloat("posy", &a[ui]->pos2[lp]->y, 0.1f, -100.0f, 100.0f);
		ImGui::DragFloat("posz", &a[ui]->pos2[lp]->z, 0.1f, -100.0f, 100.0f);
		ImGui::DragInt("meshs", &a[ui]->mp, 1, 0, 25);
	}
	ImGui::DragFloat("Alpha", &alpha, 0.1f, 0.0f, 1.0f);
	ImGui::DragInt("switc_on", &switch_on, 1, 0, 3);
	ImGui::End();
	ImGui::Begin("Engine screen");
	ImGui::InputText("model path", texbox, IM_ARRAYSIZE(texbox));
	if (ImGui::ListBoxHeader(co.c_str()))
	{
		if (ImGui::Selectable("BACK"))
		{
			this->m = 0;
			this->rize();
			cou = "BACK";
		}
		if (ImGui::Selectable("FRONT"))
		{
			this->m = 1;
			this->rize();
			cou = "FRONT";
		}
		if (ImGui::Selectable("NOCE"))
		{
			this->m = 9999;
			this->rize();
			cou = "NOCE";
		}
		if (ImGui::Selectable("SOLID"))
		{
			this->k = false;
			this->rize();
		}
		if (ImGui::Selectable("WIREFRAME"))
		{
			this->k = true;
			this->rize();
		}
		ImGui::ListBoxFooter();
	}
	if (ImGui::ListBoxHeader("Texture"))
	{
		if (ImGui::BeginCombo("Filter Mode",sbkwf.c_str()))
		{
			if (ImGui::Selectable("anisotropic"))
			{
				sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
				sbkwf = "anisotropic";
			}
			if (ImGui::Selectable("linear"))
			{
				sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				sbkwf = "linear";
			}
			if (ImGui::Selectable("point"))
			{
				sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
				sbkwf = "point";
			}
			ImGui::EndCombo();
		}
		if (ImGui::Selectable("Enable Texture"))
		{
			sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			sampDesc.MinLOD = -3.4f;
			sampDesc.MaxLOD = 3.4f;
			this->device->CreateSamplerState(&sampDesc, this->samplerState.ReleaseAndGetAddressOf());
		}
		ImGui::ListBoxFooter();
	}
	ImGui::End();
	//Assemble Together Draw Data
	ImGui::Render();
	//Render Draw Data
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	this->swapchain->Present(0, NULL);
}

bool Graphics::InitializeDirectX(HWND hwnd)
{
	try
	{
		std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

		if (adapters.size() < 1)
		{
			ErrorLogger::Log("No IDXGI Adapters found.");
			return false;
		}
		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

		scd.BufferDesc.Width = this->windowWidth;
		scd.BufferDesc.Height = this->windowHeight;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		scd.SampleDesc.Count = 8;
		scd.SampleDesc.Quality = 0;

		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 1;
		scd.OutputWindow = hwnd;
		scd.Windowed = TRUE;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT hr;
		hr = D3D11CreateDeviceAndSwapChain(adapters[0].pAdapter, //IDXGI Adapter
			D3D_DRIVER_TYPE_UNKNOWN,
			NULL, //FOR SOFTWARE DRIVER TYPE
			NULL, //FLAGS FOR RUNTIME LAYERS
			NULL, //FEATURE LEVELS ARRAY
			0, //# OF FEATURE LEVELS IN ARRAY
			D3D11_SDK_VERSION,
			&scd, //Swapchain description
			this->swapchain.GetAddressOf(), //Swapchain Address
			this->device.GetAddressOf(), //Device Address
			NULL, //Supported feature level
			this->deviceContext.GetAddressOf()); //Device Context Address

		COM_ERROR_IF_FAILED(hr, "Failed to create sampler state.");

		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		hr = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		COM_ERROR_IF_FAILED(hr, "Failed to create sampler state.");

		hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create sampler state.");

		//Describe our Depth/Stencil Buffer
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = this->windowWidth;
		depthStencilDesc.Height = this->windowHeight;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 8;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		hr = this->device->CreateTexture2D(&depthStencilDesc, NULL, this->depthStencilBuffer.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create sampler state.");

		hr = this->device->CreateDepthStencilView(this->depthStencilBuffer.Get(), NULL, this->depthStencilView.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create sampler state.");

		this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());

		//Create depth stencil state
		D3D11_DEPTH_STENCIL_DESC depthstencildesc;
		ZeroMemory(&depthstencildesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

		depthstencildesc.DepthEnable = true;
		depthstencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		depthstencildesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		hr = this->device->CreateDepthStencilState(&depthstencildesc, this->depthStencilState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create sampler state.");

		//Create the Viewport
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = this->windowWidth;
		viewport.Height = this->windowHeight;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		//Set the Viewport
		this->deviceContext->RSSetViewports(1, &viewport);

		//Create Rasterizer State
		D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		hr = this->device->CreateRasterizerState(&rasterizerDesc, this->rasterizerState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create sampler state.");
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));

		D3D11_RENDER_TARGET_BLEND_DESC rtbd;
		ZeroMemory(&rtbd, sizeof(rtbd));

		rtbd.BlendEnable = true;
		rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;
		blendDesc.RenderTarget[0] = rtbd;

		hr = this->device->CreateBlendState(&blendDesc, this->blendState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create sampler state.");
		spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());
		spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"Data\\Fonts\\comic_sans_ms_16.spritefont");

		//Create sampler description for sampler state
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = -3.4f;
		sampDesc.MaxLOD = 3.4f;
		hr = this->device->CreateSamplerState(&sampDesc, this->samplerState.GetAddressOf()); //Create sampler state
		COM_ERROR_IF_FAILED(hr, "Failed to create sampler state.");
	}
	catch (COMException & exception)
	{
		ErrorLogger::Log(exception);
		return false;
	}
	return true;
}

bool Graphics::InitializeShaders()
{

	std::wstring shaderfolder = L"";
#pragma region DetermineShaderPath
	if (IsDebuggerPresent() == TRUE)
	{
#ifdef _DEBUG //Debug Mode
	#ifdef _WIN64 //x64
			shaderfolder = L"..\\x64\\Debug\\";
	#else  //x86 (Win32)
			shaderfolder = L"..\\Debug\\";
	#endif
	#else //Release Mode
	#ifdef _WIN64 //x64
			shaderfolder = L"..\\x64\\Release\\";
	#else  //x86 (Win32)
			shaderfolder = L"..\\Release\\";
	#endif
#endif
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"BONES", 0, DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
	};

	UINT numElements = ARRAYSIZE(layout);

	if (!vertexshader.Initialize(this->device, shaderfolder + L"vertexshader.cso", layout, numElements))
		return false;

	if (!pixelshader.Initialize(this->device, shaderfolder + L"pixelshader.cso"))
		return false;


	return true;
}

bool Graphics::InitializeScene()
{
	//Textured Square
	try
	{
		//HRESULT hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\seamless_grass.jpg", nullptr, grassTexture.GetAddressOf());
		//COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");
		HRESULT hr = this->cb_vs_vertexshader.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");
		hr = this->cb_ps_pixelshader.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");
		if (!model.Initialize("Data\\Objects\\Samples\\dodge_challenger.fbx", this->device.Get(), this->deviceContext.Get(),this->cb_vs_vertexshader,this->cb_ps_pixelshader,2))//"Data\\Objects\\Samples\dodge_challenger.fbx",Data\\Objects\\dancing_vampire.dae
			return false;
		camera.SetProjectionValues(90.0f, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.1f, 150.0f);
		chCH[0] = "D3D11";
		chCH[1] = "D3D11 engine";
		chCH[2] = "D3D11 engine kmp";
		for (int i = 0; i < 3; i++)
		{
			a[i] = new Model;
			tt[i] = new XMFLOAT3;
			tt[i]->x = 0.0f;
			tt[i]->y = 0.0f;
			tt[i]->z = 0.0f;
		}
	}
	catch (COMException & exception)
	{
		ErrorLogger::Log(exception);
		return false;
	}
	return true;
}


void Graphics::rizemove(int W)
{
	this->a[W]->SetPosition(tt[W]->x, tt[W]->y, tt[W]->z);
}

void Graphics::rize()
{
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	if (!k)
	{
		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	}
	else
	{
		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
	}
	switch (this->m)
	{
	case 0:
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		break;
	case 1:
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		break;
	default:
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	}
	this->device->CreateRasterizerState(&rasterizerDesc, this->rasterizerState.ReleaseAndGetAddressOf());
}

void Graphics::rizemodel()
{
	a[TTGGGG]->g = false;
	a[TTGGGG]->Initialize(texbox, this->device.Get(), this->deviceContext.Get(), this->cb_vs_vertexshader, this->cb_ps_pixelshader,TTGGMF);
	TTGGGG += 1;
}
