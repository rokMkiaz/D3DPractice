#pragma once
class Tessellation : public Renderer
{
public:
	typedef VertexTextureNormalTangent MeshVertex;
public:
	Tessellation(Shader* shader, MeshVertex* vertices , UINT vertexCount);
	~Tessellation();

	void Update() override;
	void Render() override;

	//void AddTexture(wstring file);
	void AddTexture(wstring file);

private:
	struct Desc
	{
		Matrix TessellationVP;
		float TessellationFactor[3];
		float Tessellationinside;
	}desc;

	struct VertexTessellation
	{
		Vector3 position;
		Vector3 color;
	};


private:
	ConstantBuffer* tessellationBuffer = NULL;
	ID3DX11EffectConstantBuffer* sBuffer = NULL;

	Texture* texture = NULL;
	ID3DX11EffectShaderResourceVariable* sDiffuseMap;

	VertexTessellation* vertices;
	UINT vertexCount;

};