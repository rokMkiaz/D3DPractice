#pragma once
class TextureBuffer : public CsResource //2차원 배열을 CS에서 사용할 수 있도록 해주는 버퍼
{
public:
	TextureBuffer(ID3D11Texture2D* src);
	~TextureBuffer();

private:
	void CreateSRV() override;

	void CreateOutput() override;
	void CreateUAV() override;

	void CreateResult() override;

public:
	UINT Width() { return width; }
	UINT Height() { return height; }
	UINT ArraySize() { return arraySize; }

	ID3D11Texture2D* Output() { return (ID3D11Texture2D*)output; }
	ID3D11ShaderResourceView* OutputSRV() { return outputSRV; }

	void CopyToInput(ID3D11Texture2D* texture);
	ID3D11Texture2D* CopyFromOutput();
	ID3D11Texture2D* Result() { return (ID3D11Texture2D*)result; }

private:
	UINT width, height, arraySize;
	DXGI_FORMAT format;

	ID3D11ShaderResourceView* outputSRV;
};