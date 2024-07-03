#include "Engine.h"
char buff[1024] = { '3.14' };
float sz = 3.14f;

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	timer.Start();
	
	if (!this->render_window.Initialize(this, hInstance, window_title, window_class, width, height))
		return false;

	if (!gfx.Initialize(this->render_window.GetHWND(), width, height))
		return false;

	return true;
}

bool Engine::ProcessMessages()
{
	return this->render_window.ProcessMessages();
}

void Engine::Update()
{
	float dt = timer.GetMilisecondsElapsed();
	timer.Restart();

	while (!keyboard.CharBufferIsEmpty())
	{
		unsigned char ch = keyboard.ReadChar();
	}

	while (!keyboard.KeyBufferIsEmpty())
	{
		KeyboardEvent kbe = keyboard.ReadKey();
		unsigned char keycode = kbe.GetKeyCode();
	}

	while (!mouse.EventBufferIsEmpty())
	{
		MouseEvent me = mouse.ReadEvent();
		if (mouse.IsRightDown())
		{
			if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
			{
				this->gfx.camera.AdjustRotation((float)me.GetPosY() * 0.01f, (float)me.GetPosX() * 0.01f, 0);
			}
		}
	}
	this->gfx.model.AdjustRotation(0, 0.001f*dt, 0);
	float cameraSpeed = 0.03f;
	if (keyboard.KeyIsPressed(VK_SHIFT))
	{
		cameraSpeed = 0.07f;
	}
	if (keyboard.KeyIsPressed('W'))
	{
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetForwardVector() * cameraSpeed * dt);
	}
	if (keyboard.KeyIsPressed('S'))
	{
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetBackwardVector() * cameraSpeed * dt);
	}
	if (keyboard.KeyIsPressed('A'))
	{
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetLeftVector() * cameraSpeed * dt);
	}
	if (keyboard.KeyIsPressed('D'))
	{
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetRightVector() * cameraSpeed * dt);
	}
	if (keyboard.KeyIsPressed('K'))
	{
		send(Sclient, (const char*)&sz, sizeof(float), 0);
		//send(Sclient, buff, strlen(buff), 0);
	}
	if (keyboard.KeyIsPressed(VK_SPACE))
	{
		this->gfx.camera.AdjustPosition(0.0f, cameraSpeed * dt, 0.0f);
	}
	if (keyboard.KeyIsPressed('Z'))
	{
		this->gfx.camera.AdjustPosition(0.0f, -cameraSpeed * dt, 0.0f);
	}
}

void Engine::RenderFrame()
{
	this->gfx.RenderFrame();
}

std::string Engine::ruchar(int m)
{
	return this->gfx.chCH[m];
}

void Engine::r()
{
	sockaddr_in Sock_in;
	Sock_in.sin_family = AF_INET;
	Sock_in.sin_port = htons(12307);
	Sock_in.sin_addr.S_un.S_addr = inet_addr("192.168.31.12");
	//connect(Sclient, (const sockaddr*)&Sock_in, sizeof(sockaddr_in));
	if (connect(Sclient, (const sockaddr*)&Sock_in, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		OutputDebugStringA("sd");
	}
}
