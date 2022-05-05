#pragma once
class Tessellation : public Renderer
{
public:
	Tessellation(Shader* shader);
	~Tessellation();

	void Update() override;
	void Render() override;

	//void AddTexture(wstring file);
	void Add(Vector3& position, Vector4& color);
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
		Vector4 color;
	};


private:
	ConstantBuffer* tessellationBuffer = NULL;
	ID3DX11EffectConstantBuffer* sBuffer = NULL;

	Texture* texture = NULL;
	ID3DX11EffectShaderResourceVariable* sDiffuseMap;

	vector<VertexTessellation> vertices;

};