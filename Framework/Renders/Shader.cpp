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
	effect->GetDesc(&effectDesc);

	for (UINT i = 0; i < effectDesc.Techniques; i++)
	{
		Technique technique;
		technique.iTechnique = effect->GetTechniqueByIndex(i);
		technique.iTechnique->GetDesc(&technique.desc);
		technique.name = String::ToWString(technique.desc.Name);

		for (UINT j = 0; j < technique.desc.Passes; j++)
		{
			Pass pass;
			pass.iPass = technique.iTechnique->GetPassByIndex(j);
			pass.iPass->GetDesc(&pass.desc);
			pass.name = String::ToWString(pass.desc.Name);
			pass.iPass->GetVertexShaderDesc(&pass.passVSDesc);
			pass.passVSDesc.pShaderVariable->GetShaderDesc(pass.passVSDesc.ShaderIndex, &pass.effectVSDesc);
		
			for (UINT k = 0; k < pass.effectVSDesc.NumInputSignatureEntries; k++)
			{
				D3D11_SIGNATURE_PARAMETER_DESC desc;

				HRESULT hr = pass.passVSDesc.pShaderVariable->GetInputSignatureElementDesc(pass.passVSDesc.ShaderIndex, k, &desc);
				Check(hr);

				pass.signatureDescs.push_back(desc);
			}
			pass.inputLayout = CreateInputLayout(fxBlob, &pass.effectVSDesc, pass.signatureDescs);
			pass.stateBlock = initialStateBlock;

			technique.passes.push_back(pass);
		
		}
		techniques.push_back(technique);
	}

	for (UINT i = 0; i < effectDesc.ConstantBuffers; i++)
	{
		ID3DX11EffectConstantBuffer* iBuffer;
		iBuffer = effect->GetConstantBufferByIndex(i); //STDMETHOD(method)라는 녀석은 virtual HRESULT STDMETHODCALLTYPE method 인데

		D3DX11_EFFECT_VARIABLE_DESC vDesc;
		iBuffer->GetDesc(&vDesc);

		int a = 0;
	}

	for (UINT i = 0; i < effectDesc.GlobalVariables; i++)
	{
		ID3DX11EffectVariable* iVariable;
		iVariable = effect->GetVariableByIndex(i);

		D3DX11_EFFECT_VARIABLE_DESC vDesc;
		iVariable->GetDesc(&vDesc);

		int a = 0;
	}

	SafeRelease(fxBlob);

}

//IA당시 정점 구성성분 특성에 따른 생성
ID3D11InputLayout* Shader::CreateInputLayout(ID3DBlob* fxBlob, D3DX11_EFFECT_SHADER_DESC* effectVsDesc, vector<D3D11_SIGNATURE_PARAMETER_DESC>& params)
{ 

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;

	for (D3D11_SIGNATURE_PARAMETER_DESC& paramDesc : params)
	{
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		string name = paramDesc.SemanticName;
		transform(name.begin(), name.end(), name.begin(), toupper);//특정 함수를 써서 값을 변경할 때 사용  /DirectTex.h 
	
		if (name == "POSITION")
		{
			elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			//elementDesc.InputSlot = paramDesc.SemanticIndex;
		}
	}
}


