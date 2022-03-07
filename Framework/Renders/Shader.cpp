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
	if (Path::ExistFile(file) == false)
	{
		MessageBox(NULL, file.c_str(), L"파일을 찾을 수 없음", MB_OK);
		assert(false);
	} 
	//Shader 읽기
	ID3DBlob* fxBlob;
	if (Path::GetExtension(file) == L"fx")
	{
		ID3DBlob* error;
		INT flag = D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY | D3D10_SHADER_PACK_MATRIX_ROW_MAJOR;

		HRESULT hr = D3DCompileFromFile(file.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, NULL, "fx_5_0", flag, NULL, &fxBlob, &error);
		if (FAILED(hr))
		{
			if (error != NULL)
			{
				string str = (const char*)error->GetBufferPointer();
				MessageBoxA(NULL, str.c_str(), "Shader Error", MB_OK);
			}
			assert(false);
		}
	}
	else if (Path::GetExtension(file) == L"fxo")
	{
		HANDLE fileHandle = CreateFile(file.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		bool bChecked = fileHandle != INVALID_HANDLE_VALUE;
		assert(bChecked);



		DWORD dataSize = GetFileSize(fileHandle, NULL);
		assert(dataSize != 0xFFFFFFFF);

		void* data = malloc(dataSize);
		DWORD readSize;
		Check(ReadFile(fileHandle, data, dataSize, &readSize, NULL));

		CloseHandle(fileHandle);
		fileHandle = NULL;

		D3DCreateBlob(dataSize, &fxBlob);
		memcpy(fxBlob->GetBufferPointer(), data, dataSize);
	}
	else
	{
		wstring errorMsg = wstring(L"이펙트 파일이 아님 : ") + file;
		MessageBox(NULL, errorMsg.c_str(), L"Shader Error", MB_OK);

		assert(false);
	}
	Check(D3DX11CreateEffectFromMemory(fxBlob->GetBufferPointer(), fxBlob->GetBufferSize(), 0, D3D::GetDevice(), &effect));

}
