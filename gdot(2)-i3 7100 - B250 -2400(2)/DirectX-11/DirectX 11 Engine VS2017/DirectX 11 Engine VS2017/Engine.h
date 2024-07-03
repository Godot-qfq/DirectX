#pragma once
#include "WindowContainer.h"
#include "Timer.h"
#include "..\\DirectX 11 Engine VS2017\Scokentint.hpp"
#pragma warning(disable:4996)

class Engine : WindowContainer
{
public:
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	bool ProcessMessages();
	void Update();
	void RenderFrame();
	std::string ruchar(int m);
	void r();
private:
	Timer timer;
	ScoketInit sockeInit;
	SOCKET Sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
};