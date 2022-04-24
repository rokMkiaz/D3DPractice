#pragma once
class Tessellation : public Renderer
{
public:
	Tessellation(Shader* shader);
	~Tessellation();

	void Render();


private:
	struct OutputConstantHS   
	{
		float	edges[3];
		float	inside;
	};

	struct OutputHS
	{
		float pos;
		Vector2 Scale;
		UINT MapIndex;
	};
	struct OutputDS
	{

		Vector4	pos;
	};

private:
	VertexBuffer* vertexBuffer = NULL;
	IndexBuffer* indexBuffer = NULL;

	UINT* indices;

	UINT vertexCount, indexCount;


};