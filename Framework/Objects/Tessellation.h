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

private:
	struct Desc
	{
		Matrix TessellationVP;

		float TessellationFactor;
		float Padding[3];
	}desc;

	struct VertexTessellation
	{
		Vector3 position;
		Vector4 color;
	};


private:
	ConstantBuffer* tessellationBuffer = NULL;
	ID3DX11EffectConstantBuffer* sBuffer = NULL;



	vector<VertexTessellation> vertices;

};