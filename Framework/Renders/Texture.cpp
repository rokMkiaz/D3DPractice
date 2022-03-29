#include "framework.h"
#include "Texture.h"

using namespace DirectX;

vector<TextureDesc> Textures::descs;

Texture::Texture(wstring file, D3DX11_IMAGE_LOAD_INFO* loadInfo)
	: file(file)
{
	bool b = Path::IsRelativePath(file);
	if (b == true)
		this->file = L"../../_Textures/" + file;

	Textures::Load(this, loadInfo);
	String::Replace(&this->file, L"../../_Textures", L"");
}
Texture::~Texture()
{

}

void Texture::SaveFile(wstring file)
{
	ID3D11Texture2D* srcTexture;
	view->GetResource((ID3D11Resource**)&srcTexture);

	SaveFile(file, srcTexture);
}

ID3D11Texture2D* Texture::GetTexture()
{
	ID3D11Texture2D* texture;
	view->GetResource((ID3D11Resource**)&texture);

	return texture;
}
void Texture::SaveFile(wstring file, ID3D11Texture2D* src)
{
	D3D11_TEXTURE2D_DESC srcDesc;
	src->GetDesc(&srcDesc);

	ID3D11Texture2D* dest;
	D3D11_TEXTURE2D_DESC destDesc;
	ZeroMemory(&destDesc, sizeof(D3D11_TEXTURE2D_DESC));
	destDesc.Width = srcDesc.Width;
	destDesc.Height = srcDesc.Height;
	destDesc.MipLevels = 1;
	destDesc.ArraySize = 1;
	destDesc.Format = srcDesc.Format;
	destDesc.SampleDesc = srcDesc.SampleDesc;
	destDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	destDesc.Usage = D3D11_USAGE_STAGING;

	HRESULT hr;
	hr = D3D::GetDevice()->CreateTexture2D(&destDesc, NULL, &dest);
	Check(hr);

	hr = D3DX11LoadTextureFromTexture(D3D::GetDC(), src, NULL, dest);
	Check(hr);


	wstring ext = Path::GetExtension(file);
	transform(ext.begin(), ext.end(), ext.begin(), toupper);

	if (ext == L"BMP")
		hr = D3DX11SaveTextureToFile(D3D::GetDC(), dest, D3DX11_IFF_BMP, file.c_str());
	else
		hr = D3DX11SaveTextureToFile(D3D::GetDC(), dest, D3DX11_IFF_PNG, file.c_str());


	Check(hr);

	SafeRelease(dest);
}