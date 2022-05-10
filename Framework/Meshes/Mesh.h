#pragma once

class Mesh 
{
public:
	typedef VertexTextureNormalTangent MeshVertex;

public:
	Mesh();
	virtual ~Mesh();


	void SetShader(Shader* shader);
	void Pass(UINT val) { pass = val; }

	void Update();
	void Render(UINT drawCount);

	
	MeshVertex* GetVerticesData() { return vertices; }
	UINT GetVertexCount() { return vertexCount; }

	UINT* GetIndicesData() { return indices; }
	UINT GetIndexCount() { return indexCount; }



protected:
	virtual void Create() = 0;
	

protected:
	Shader* shader;
	UINT pass = 0;

	PerFrame* perFrame = NULL;

	VertexBuffer* vertexBuffer = NULL;
	IndexBuffer* indexBuffer = NULL;


	MeshVertex* vertices;
	UINT* indices;

	UINT vertexCount, indexCount;

};