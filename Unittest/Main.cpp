#include "stdafx.h"
#include "Main.h"
#include "Systems/Window.h"


#include"TEST/TextureLoadDemo.h"
#include"TEST/HeightMapDemo.h"

void Main::Initialize()
{
	Push(new HeightMapDemo);
	//Push(new TextureLoadDemo);
}

void Main::Ready()
{

}

void Main::Destroy()
{
	for (IExecute* exe : executes)
	{
		exe->Destroy();
		SafeDelete(exe);
	}
}

void Main::Update()
{
	for (IExecute* exe : executes)
		exe->Update();

}

void Main::PreRender()
{
	for (IExecute* exe : executes)
		exe->PreRender();


}

void Main::Render()
{
	for (IExecute* exe : executes)
		exe->Render();

}

void Main::PostRender()
{
	for (IExecute* exe : executes)
		exe->PostRender();

}

void Main::ResizeScreen()
{

	for (IExecute* exe : executes)
		exe->ResizeScreen();

}

void Main::Push(IExecute* execute)
{
	executes.push_back(execute);

	execute->Initialize();
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR param, int command)
{
	D3DDesc desc;
	desc.appName = L"D3D Game";
	desc.instance = instance;
	desc.bFullScreen = false;
	desc.bVsync = false;
	desc.handle = NULL;
	desc.width = 1280;
	desc.height = 720;
	desc.background = Color(0.3f, 0.3f, 0.3f, 1.0f);
	D3D::SetDesc(desc);

	Main* main = new Main();
	WPARAM wParam = Window::Run(main);

	SafeDelete(main);

	return wParam;
}