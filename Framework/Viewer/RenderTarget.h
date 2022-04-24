#pragma once

class RenderTarget
{
public:
	RenderTarget(UINT width = 0, UINT height = 0, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
	~RenderTarget();

	ID3D11RenderTargetView* RTV() { return rtv; }
	ID3D11ShaderResourceView* SRV() { return srv; }

	void SaveTexture(wstring file);

	void PreRender(class DepthStencil* depthStencil); //OM에서 Rtv와 DSV의 텍스처 크기가 다르면 렌더링되지 않으므로 받아둬야함
	static void PreRender(RenderTarget** targets, UINT count, class DepthStencil* depthStencil);

private:
	UINT width, height;
	DXGI_FORMAT format;

	ID3D11Texture2D* texture;
	ID3D11RenderTargetView* rtv;
	ID3D11ShaderResourceView* srv;
};