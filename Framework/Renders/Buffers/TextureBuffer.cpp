#include"Framework.h"
#include"TextureBuffer.h"

TextureBuffer::TextureBuffer(ID3D11Texture2D* src)
{
	D3D11_TEXTURE2D_DESC srcDesc;
	src->GetDesc(&srcDesc);

	width = srcDesc.Width;
	height = srcDesc.Height;
	arraySize = srcDesc.ArraySize;
	format = srcDesc.Format;


	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = width;
	desc.Height = height;
	desc.ArraySize = arraySize;
	desc.Format = format;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;

	ID3D11Texture2D* texture = NULL;
	Check(D3D::GetDevice()->CreateTexture2D(&desc, NULL, &texture));
	D3D::GetDC()->CopyResource(texture, src);

	input = (ID3D11Resource*)texture;

	CreateBuffer();
}

TextureBuffer::~TextureBuffer()
{

}

void TextureBuffer::CreateSRV()
{
	ID3D11Texture2D* texture = (ID3D11Texture2D*)input;

	D3D11_TEXTURE2D_DESC desc;
	texture->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.ArraySize = arraySize;//ÅØ½ºÃ³ 2D Array

	Check(D3D::GetDevice()->CreateShaderResourceView(texture, &srvDesc, &srv));
}

void TextureBuffer::CreateOutput()
{
	ID3D11Texture2D* texture = NULL;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = width;
	desc.Height = height;
	desc.ArraySize = arraySize;
	desc.Format = format;
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	Check(D3D::GetDevice()->CreateTexture2D(&desc, NULL, &texture));

	output = (ID3D11Resource*)texture;


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.ArraySize = arraySize;

	Check(D3D::GetDevice()->CreateShaderResourceView(texture, &srvDesc, &outputSRV));
}

void TextureBuffer::CreateUAV()
{
	ID3D11Texture2D* texture = (ID3D11Texture2D*)output;

	D3D11_TEXTURE2D_DESC desc;
	texture->GetDesc(&desc);


	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	uavDesc.Texture2DArray.ArraySize = arraySize;

	Check(D3D::GetDevice()->CreateUnorderedAccessView(texture, &uavDesc, &uav));
}

void TextureBuffer::CreateResult()
{
	ID3D11Texture2D* texture;

	D3D11_TEXTURE2D_DESC desc;
	((ID3D11Texture2D*)output)->GetDesc(&desc);
	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;

	Check(D3D::GetDevice()->CreateTexture2D(&desc, NULL, &texture));

	result = (ID3D11Resource*)texture;
}

void TextureBuffer::CopyToInput(ID3D11Texture2D* texture)
{
	D3D::GetDC()->CopyResource(input, texture);
}

ID3D11Texture2D* TextureBuffer::CopyFromOutput()
{
	D3D::GetDC()->CopyResource(result, output);

	return (ID3D11Texture2D*)result;
}
