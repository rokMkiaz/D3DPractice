#pragma once
class Shadow
{
public:
	Shadow(Shader* shader, Vector3& position, float radius, UINT width = 1024, UINT height = 1024);
	~Shadow();

	void PreRender();

	ID3D11ShaderResourceView* SRV() { return renderTarget->SRV(); }

private:
	void CalcViewProjection(); //그림자가 그려질 공간 계산

private:
	struct Desc
	{
		Matrix View; //주 조명
		Matrix Projection;

		Vector2 MapSize; //퀄리티
		float Bias = -0.0006f;

		UINT Quality = 0; //퀄리티
	} desc;

private:
	Shader* shader;
	UINT width, height;

	Vector3 position; // 조명이 어디를 비추는가
	float radius; //조명 구역

	RenderTarget* renderTarget;
	DepthStencil* depthStencil;
	Viewport* viewport;

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;
	ID3DX11EffectShaderResourceVariable* sShadowMap;//그림자 깊이를 렌더링 했을때 2Pass정보 전달할 변수

	ID3D11SamplerState* pcfSampler;//PCF
	ID3DX11EffectSamplerVariable* sPcfSampler;
};