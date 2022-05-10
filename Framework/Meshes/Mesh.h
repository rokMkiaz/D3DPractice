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

<<<<<<< HEAD
=======

>>>>>>> 6a3dabf1d266e1424da246b3aee6e75efb055cb3

protected:
	virtual void Create() = 0;
	

protected:
	Shader* shader;
	UINT pass = 0;

	PerFrame* perFrame = NULL;

	VertexBuffer* vertexBuffer = NULL;
	IndexBuffer* indexBuffer = NULL;

<<<<<<< HEAD

=======
>>>>>>> 6a3dabf1d266e1424da246b3aee6e75efb055cb3

	MeshVertex* vertices;
	UINT* indices;

	UINT vertexCount, indexCount;

};