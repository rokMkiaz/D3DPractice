#pragma once
class ConstantBuffer : public iBuffer
{
public:
	ConstantBuffer(void* data, UINT dataSize);
	~ConstantBuffer();

	ID3D11Buffer* Buffer() { return buffer; }

	void Render();

private:
	ID3D11Buffer* buffer;
	
	ID3D11HullShader*   hullShader = nullptr ;
	ID3D11DomainShader* domainShader= nullptr;

	void* data;
	UINT dataSize;

};
