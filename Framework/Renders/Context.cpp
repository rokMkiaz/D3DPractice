#include "Framework.h"
#include "Context.h"
#include "Viewer/Viewport.h"
#include "Viewer/Perspective.h"
#include "Viewer/Freedom.h"

Context* Context::instance = NULL;

Context* Context::Get()
{
	//assert(instance != NULL);

	return instance;
}
void Context::Create()
{
	assert(instance == NULL);

	instance = new Context();
}
void Context::Delete()
{
	SafeDelete(instance);
}
Context::Context()
{
	D3DDesc desc = D3D::GetDesc();

	perspective = new Perspective(desc.Width, desc.Height);
	viewport = new Viewport(desc.Width, desc.Height);
	camera = new Freedom();
}
Context::~Context()
{
	SafeDelete(perspective);
	SafeDelete(viewport);
	SafeDelete(camera);
}
