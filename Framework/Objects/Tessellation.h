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

private:



	VertexTessellation* vertices;
	UINT* indices;
};