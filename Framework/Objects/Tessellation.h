#pragma once
class Tessellation : public Renderer
{
public:
	typedef VertexTextureNormalTangent TessellationVertex;
public:
	Tessellation(Shader* shader, VertexTextureNormalTangent* vertices, UINT vertexCount,UINT&indices,UINT indexCount);
	~Tessellation();

	void Update() override;
	void Render() override;

	//void AddTexture(wstring file);


private:
	ConstantBuffer* TessellationBuffer;
	

	TessellationVertex* vertices;
	UINT* indices;
};