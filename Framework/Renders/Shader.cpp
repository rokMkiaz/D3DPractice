#include"Framework.h"
#include"Shader.h"

Shader::Shader(wstring file)
	:file(L"../../_Shaders/"+file)
{
	initialStateBlock = new StateBlock();
	{
		D3D::GetDC()->RSGetState(&initialStateBlock->rsRasterizerState);
		D3D::GetDC()->OMGetBlendState(&initialStateBlock->omBlendState, initialStateBlock->omBlendFactor,&initialStateBlock->omSampleMask);
		D3D::GetDC()->OMGetDepthStencilState(&initialStateBlock->omDepthStencilState, &initialStateBlock->omStencilRef);
	}
	CreateEffect();
}

Shader::~Shader()
{
	for (Technique& temp : techniques)
	{
		for (Pass& pass : temp.passes)
		{
			SafeRelease(pass.inputLayout);
		}
	}

	SafeDelete(initialStateBlock);
	SafeRelease(effect);
}

void Shader::CreateEffect()
{
	
}
