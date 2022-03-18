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