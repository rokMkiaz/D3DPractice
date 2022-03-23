#include"Framework.h"
#include"RawBuffer.h"

RawBuffer::RawBuffer(void* inputData, UINT inputByte, UINT outputByte)
	: CsResource()
	, inputData(inputData), inputByte(inputByte), outputByte(outputByte)
{
	CreateBuffer();
}

RawBuffer::~RawBuffer()
{
}

void RawBuffer::CreateInput()
{
	if (inputByte < 1) return;

	ID3D11Buffer* buffer = NULL;
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	desc.ByteWidth = inputByte;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;//버퍼 용도
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = inputData;

	Check(D3D::GetDevice()->CreateBuffer(&desc, inputData != NULL ? &subResource : NULL, &buffer));

	input = (ID3D11Resource*)buffer;
}

void RawBuffer::CreateSRV()
{
	if (inputByte < 1) return;


	ID3D11Buffer* buffer = (ID3D11Buffer*)input;

	D3D11_BUFFER_DESC desc;
	buffer->GetDesc(&desc);


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = DXGI_FORMAT_R32_TYPELESS; //프로그램에 맡기겠다.
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;//Raw버퍼용도로 사용한다
	srvDesc.BufferEx.NumElements = desc.ByteWidth / 4;

	Check(D3D::GetDevice()->CreateShaderResourceView(buffer, &srvDesc, &srv));
}

void RawBuffer::CreateOutput()
{
	ID3D11Buffer* buffer = NULL;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	desc.ByteWidth = outputByte;
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS; //UAV연결
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

	Check(D3D::GetDevice()->CreateBuffer(&desc, NULL, &buffer));

	output = (ID3D11Resource*)buffer;
}

void RawBuffer::CreateUAV()
{
	ID3D11Buffer* buffer = (ID3D11Buffer*)output;

	D3D11_BUFFER_DESC desc;
	buffer->GetDesc(&desc);


	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
	uavDesc.Buffer.NumElements = desc.ByteWidth / 4;

	Check(D3D::GetDevice()->CreateUnorderedAccessView(buffer, &uavDesc, &uav));
}

void RawBuffer::CreateResult()
{
	ID3D11Buffer* buffer;

	D3D11_BUFFER_DESC desc;
	((ID3D11Buffer*)output)->GetDesc(&desc);
	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.BindFlags = D3D11_USAGE_DEFAULT;//0이안되면 UNORDERED_ACCESS에서 디폴트가 아니면 fail시켜버린다.
	desc.MiscFlags = 0;

	Check(D3D::GetDevice()->CreateBuffer(&desc, NULL, &buffer));

	result = (ID3D11Resource*)buffer;

}

void RawBuffer::CopyToInput(void* data)
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(input, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, data, inputByte);
	}
	D3D::GetDC()->Unmap(input, 0);
}

void RawBuffer::CopyFromOuput(void* data)
{
	D3D::GetDC()->CopyResource(result, output);

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(result, 0, D3D11_MAP_READ, 0, &subResource);
	{
		memcpy(data, subResource.pData, outputByte);
	}
	D3D::GetDC()->Unmap(result, 0);
}