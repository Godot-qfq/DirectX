#include "Engine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

	HRESULT hr = CoInitialize(NULL);
	Engine engine;
	//engine.r();
	if (engine.Initialize(hInstance, "Title", "MyWindowClass", 1500, 900))
	{
		while (engine.ProcessMessages() == true)
		{
			engine.Update();
			engine.RenderFrame();
		}
		std::ofstream fout;
		fout.open("test.txt");
		for (int m = 0; m < 3; m++)
		{
			fout << engine.ruchar(m);
			fout << "\n";
		}
		fout.close();
		//OutputDebugStringA("sadsda\n");
	}
	return 0;
}