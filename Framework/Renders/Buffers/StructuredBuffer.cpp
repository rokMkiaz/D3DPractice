#include"Framework.h"
#include"StructuredBuffer.h"

StructuredBuffer::StructuredBuffer(void* inputData, UINT inputStride, UINT inputCount, UINT outputStride, UINT outputCount)
	: CsResource()
	, inputData(inputData)
	, inputStride(inputStride), inputCount(inputCount)
	, outputStride(outputStride), outputCount(outputCount)
{
	if (outputStride == 0 || outputCount == 0)
	{
		this->outputStride = inputStride;
		this->outputCount = inputCount;
	}

	CreateBuffer();
}

StructuredBuffer::~StructuredBuffer()
{

}

void StructuredBuffer::CreateInput()
{
	ID3D11Buffer* buffer = NULL;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	desc.ByteWidth = InputByteWidth();
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = inputStride;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = inputData;

	Check(D3D::GetDevice()->CreateBuffer(&desc, inputData != NULL ? &subResource : NULL, &buffer));

	input = (ID3D11Resource*)buffer;
}

void StructuredBuffer::CreateSRV()
{
	ID3D11Buffer* buffer = (ID3D11Buffer*)input;

	D3D11_BUFFER_DESC desc;
	buffer->GetDesc(&desc);


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.NumElements = inputCount;

	Check(D3D::GetDevice()->CreateShaderResourceView(buffer, &srvDesc, &srv));
}

void StructuredBuffer::CreateOutput()
{
	ID3D11Buffer* buffer = NULL;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	desc.ByteWidth = OutputByteWidth();
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = outputStride;

	Check(D3D::GetDevice()->CreateBuffer(&desc, NULL, &buffer));

	output = (ID3D11Resource*)buffer;
}

void StructuredBuffer::CreateUAV()
{
	ID3D11Buffer* buffer = (ID3D11Buffer*)output;

	D3D11_BUFFER_DESC desc;
	buffer->GetDesc(&desc);


	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.NumElements = outputCount;

	Check(D3D::GetDevice()->CreateUnorderedAccessView(buffer, &uavDesc, &uav));
}

void StructuredBuffer::CreateResult()
{
	ID3D11Buffer* buffer;

	D3D11_BUFFER_DESC desc;
	((ID3D11Buffer*)output)->GetDesc(&desc);
	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;

	Check(D3D::GetDevice()->CreateBuffer(&desc, NULL, &buffer));

	result = (ID3D11Resource*)buffer;
}

void StructuredBuffer::CopyToInput(void* data)
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(input, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, data, InputByteWidth());
	}
	D3D::GetDC()->Unmap(input, 0);
}

void StructuredBuffer::CopyFromOutput(void* data)
{
	D3D::GetDC()->CopyResource(result, output);

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(result, 0, D3D11_MAP_READ, 0, &subResource);
	{
		memcpy(data, subResource.pData, OutputByteWidth());
	}
	D3D::GetDC()->Unmap(result, 0);
}