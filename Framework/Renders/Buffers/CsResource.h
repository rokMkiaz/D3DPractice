#pragma once
#include "Framework.h"

class CsResource   //컴퓨터 내부에서 다루는 버퍼
{ 
public:
	CsResource() {};
	virtual ~CsResource()
	{
		SafeRelease(input);
		SafeRelease(srv);

		SafeRelease(output);
		SafeRelease(uav);

		SafeRelease(result);
	}

protected:
	virtual void CreateInput() {}
	virtual void CreateSRV() {}

	virtual void CreateOutput() {}
	virtual void CreateUAV() {}

	virtual void CreateResult() {}

	void CreateBuffer()
	{
		CreateInput();
		CreateSRV();

		CreateOutput();
		CreateUAV();

		CreateResult();
	}

public:
	ID3D11ShaderResourceView* SRV() { return srv; }
	ID3D11UnorderedAccessView* UAV() { return uav; }

protected:
	ID3D11Resource* input = NULL;
	ID3D11ShaderResourceView* srv = NULL; //input

	ID3D11Resource* output = NULL;
	ID3D11UnorderedAccessView* uav = NULL; //Output

	ID3D11Resource* result = NULL;
};