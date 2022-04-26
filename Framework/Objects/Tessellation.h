#pragma once
class Tessellation : public Renderer
{
public:
	Tessellation(Shader* shader, VertexTextureNormalTangent* vertices, UINT vertexCount,UINT&indices,UINT indexCount);
	~Tessellation();

	void Update() override;
	void Render() override;

	//void AddTexture(wstring file);

private:
	struct VertexTessellation
	{
		Vector3 postion;
	};
	struct Desc
	{
		Vector3 OutputHS ;
		float CB_HS_Padding;

		Vector4 OutputDS ;
	}desc;

private:
	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;


	VertexTessellation* vertices;
	UINT* indices;
};