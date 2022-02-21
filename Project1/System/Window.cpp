#include"Framework.h"
#include"IExecute.h"
#include"Window.h"

WPARAM Window::Run(IExecute* main)
{
	mainExecute = main;
	Create();


	mainExecute->Initialize();

	MSG msg = { 0 };
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);//msg변수에 키보드 메시지가 들어 있을 경우 키에 대응하는 문자생성
			DispatchMessage(&msg);
			
		
		}
		else
		{
			MainRender();
		}
	}
	mainExecute->Destory();

	Destory();
	
	 
	return msg.wParam;
}

void Window::Create()
{


	WNDCLASSEX wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hIconSm = wndClass.hIcon;
	wndClass.hInstance = NULL;
	wndClass.lpfnWndProc = (WNDPROC)WndProc;
	wndClass.lpszClassName = L"Window";
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.cbSize = sizeof(WNDCLASSEX);

	WORD wHr = RegisterClassEx(&wndClass);
	Check(wHr != 0);

	
	ShowCursor(true);
}

void Window::Destory()
{
}

LRESULT Window::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{

	if (message == WM_SIZE)
	{
		if (mainExecute != NULL)
		{
			float width = (float)LOWORD(lParam);
			float height = (float)HIWORD(lParam);

			mainExecute->ResizeScreen();
		}
	}

	if (message == WM_CLOSE || message == WM_DESTROY)
	{
		PostQuitMessage(0);

		return 0;
	}
	return DefWindowProc(handle, message, wParam, lParam);
}

void Window::MainRender()
{


	mainExecute->Update();

	mainExecute->PreRender();
}
