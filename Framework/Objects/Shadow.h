#pragma once
class Shadow
{
public:
	Shadow(Shader* shader, Vector3& position, float radius, UINT width = 1024, UINT height = 1024);
	~Shadow();

	void PreRender();

	ID3D11ShaderResourceView* SRV() { return renderTarget->SRV(); }

private:
	void CalcViewProjection(); //�׸��ڰ� �׷��� ���� ���

private:
	struct Desc
	{
		Matrix View; //�� ����
		Matrix Projection;

		Vector2 MapSize; //����Ƽ
		float Bias = -0.0006f;

		UINT Quality = 0; //����Ƽ
	} desc;

private:
	Shader* shader;
	UINT width, height;

	Vector3 position; // ������ ��� ���ߴ°�
	float radius; //���� ����

	RenderTarget* renderTarget;
	DepthStencil* depthStencil;
	Viewport* viewport;

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;
	ID3DX11EffectShaderResourceVariable* sShadowMap;//�׸��� ���̸� ������ ������ 2Pass���� ������ ����

	ID3D11SamplerState* pcfSampler;//PCF
	ID3DX11EffectSamplerVariable* sPcfSampler;
};