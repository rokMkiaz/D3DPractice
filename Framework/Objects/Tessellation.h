#pragma once
class Tessellation : public Renderer
{
public:
	Tessellation(Shader* shader);
	~Tessellation();

	void Update() override;
	void Render() override;

	void Add(Vector3& position, Vector2& scale, UINT mapIndex);
	void AddTexture(wstring file);

private:
	struct VertexTessellation
	{
		Vector3 postion;
	};

private:
	ID3DX11EffectConstantBuffer* sHullShader;
	ID3DX11EffectConstantBuffer* sDomainShader;

	vector<VertexTessellation> vertices;
	vector<UINT> indices;
};