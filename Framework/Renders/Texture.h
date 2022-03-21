#pragma once

class Texture
{
public:
	friend class Textures;

public:
	void SaveFile(wstring file);
	static void SaveFile(wstring file, ID3D11Texture2D* src);

	D3D11_TEXTURE2D_DESC ReadPixel(DXGI_FORMAT readFormat, vector<Color>* pixels);
	static D3D11_TEXTURE2D_DESC ReadPixel(ID3D11Texture2D* src, DXGI_FORMAT readFormat, vector<Color>* pixels);

public:
	Texture(wstring file, D3DX11_IMAGE_LOAD_INFO* loadInfo = NULL);
	~Texture();

	operator ID3D11ShaderResourceView* () { return view; }


	wstring GetFile() { return file; }

	UINT GetWidth() { return metaData.width; }
	UINT GetHeight() { return metaData.height; }

	void GetImageInfo(DirectX::TexMetadata* data)
	{
		*data = metaData;
	}

	ID3D11ShaderResourceView* SRV() { return view; }
	ID3D11Texture2D* GetTexture();

private:
	wstring file;

	DirectX::TexMetadata metaData;
	ID3D11ShaderResourceView* view;
};
