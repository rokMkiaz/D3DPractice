#include"Framework.h"
#include"Window.h"
#include"IExecute.h"

IExecute* Window::mainExecute = NULL;

WPARAM Window::Run(IExecute* main)
{
	mainExecute = main;
	Create();

	D3DDesc desc = D3D::GetDesc();

	D3D::Create();
	Keyboard::Create();
	Mouse::Create();

	Time::Create();
	Time::Get()->Start();

	Gui::Create();
	Context::Create();
	DebugLine::Create();

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
	mainExecute->Destroy();

	DebugLine::Delete();
	Context::Delete();
	Gui::Delete();
	Time::Delete();
	Mouse::Delete();
	Keyboard::Delete();
	D3D::Delete();

	Destory();
	
	 
	return msg.wParam;
}

void Window::Create()
{
	D3DDesc desc = D3D::GetDesc();

	WNDCLASSEX wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hIconSm = wndClass.hIcon;
	wndClass.hInstance = desc.instance;
	wndClass.lpfnWndProc = (WNDPROC)WndProc;
	wndClass.lpszClassName = desc.appName.c_str();
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.cbSize = sizeof(WNDCLASSEX);

	WORD wHr = RegisterClassEx(&wndClass);
	assert(wHr != 0);

	if (desc.bFullScreen == true)
	{
		DEVMODE devMode = { 0 };
		devMode.dmSize = sizeof(DEVMODE);
		devMode.dmPelsWidth = (DWORD)desc.width;
		devMode.dmPelsHeight = (DWORD)desc.height;
		devMode.dmBitsPerPel = 32;
		devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
	}

	desc.handle = CreateWindowEx
	(
		WS_EX_APPWINDOW,
		desc.appName.c_str(),
		desc.appName.c_str(),
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		(HMENU)NULL,
		desc.instance,
		NULL
	);
	assert(desc.handle != NULL);
	D3D::SetDesc(desc);

	RECT rect = { 0, 0, (LONG)desc.width, (LONG)desc.height };

	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - (UINT)desc.width) / 2;
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - (UINT)desc.height) / 2;

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	MoveWindow
	(
		desc.handle,
		centerX, centerY,
		rect.right - rect.left,
		rect.bottom - rect.top,
		TRUE
	);
	ShowWindow(desc.handle, SW_SHOWNORMAL);
	SetForegroundWindow(desc.handle);
	SetFocus(desc.handle);
	
	ShowCursor(true);
}

void Window::Destory()
{
	D3DDesc desc = D3D::GetDesc();

	if (desc.bFullScreen == true)
		ChangeDisplaySettings(NULL, 0);

	DestroyWindow(desc.handle);

	UnregisterClass(desc.appName.c_str(), desc.instance);
}

LRESULT Window::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	Mouse::Get()->InputProc(message, wParam, lParam);

	if (Gui::Get()->MsgProc(handle, message, wParam, lParam))
		return true;

	if (message == WM_SIZE)
	{
		if (mainExecute != NULL)
		{
			float width = (float)LOWORD(lParam);
			float height = (float)HIWORD(lParam);

			if (D3D::Get() != NULL)
				D3D::Get()->ResizeScreen(width, height);



			if (Context::Get() != NULL)
				Context::Get()->ResizeScreen();



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
	Time::Get()->Update();

	if (ImGui::IsMouseHoveringAnyWindow() == false)
	{
		Keyboard::Get()->Update();
		Mouse::Get()->Update();
	}
	Gui::Get()->Update();
	Context::Get()->Update();
	DebugLine::Get()->Update();

	mainExecute->Update();

	mainExecute->PreRender();

	D3DDesc desc = D3D::GetDesc();

	D3D::Get()->SetRenderTarget();
	D3D::Get()->Clear(desc.background);
	{		
		Context::Get()->Render();
		
		mainExecute->Render();
		DebugLine::Get()->Render();
		
		mainExecute->PostRender();
		
		Gui::Get()->Render();
	}
	D3D::Get()->Present();
}
