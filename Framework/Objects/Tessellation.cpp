#include "Framework.h"
#include "Tessellation.h"
struct OutputHS
{
	float pos;
	Vector2 Scale;
	UINT MapIndex;
};
struct OutputDS
{
	Vector4	pos;
};

Tessellation::Tessellation(Shader* shader)
	:Renderer(shader)
{
	sHullShader = shader->AsConstantBuffer("OutputHS");
	sDomainShader = shader->AsConstantBuffer("OutputDS");

}

Tessellation::~Tessellation()
{
	SafeDelete(sHullShader);
	SafeDelete(sDomainShader);
}

void Tessellation::Update()
{
	Super::Update();
}

void Tessellation::Render()
{

}

