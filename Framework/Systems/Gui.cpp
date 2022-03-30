#include"Framework.h"
#include"Gui.h"

Gui* Gui::instance = NULL;

void Gui::Create()
{
	assert(instance == NULL);

	instance = new Gui();
}
void Gui::Delete()
{
	SafeDelete(instance);
}

Gui* Gui::Get()
{
	return instance;
}
LRESULT Gui::MsgProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_Proc(handle, message, wParam, lParam);
}
void Gui::Resize()
{
	ImGui_ImplDX11_InvalidateDeviceObjects();
	ImGui_ImplDX11_CreateDeviceObjects();
}

void Gui::Update()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}
void Gui::Render()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowBgAlpha(0.0f);

	ImGui::Begin
	(
		"TextWindow", NULL,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoInputs |
		ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoNavFocus
	);
	for (GuiText text : texts)
	{
		ImVec2 position = ImVec2(text.Position.x, text.Position.y);
		ImColor color = ImColor(text.Color.r, text.Color.g, text.Color.b, text.Color.a);

		ImGui::GetWindowDrawList()->AddText(position, color, text.Content.c_str());
	}
	texts.clear();
	ImGui::End();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

}