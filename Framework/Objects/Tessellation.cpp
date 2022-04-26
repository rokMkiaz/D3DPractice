#include "Framework.h"
#include "Tessellation.h"


Tessellation::Tessellation(Shader* shader, VertexTextureNormalTangent* vertices, UINT vertexCount, UINT&indices, UINT indexCount)
	:Renderer(shader)
{
	buffer = new ConstantBuffer(&desc, sizeof(Desc));
	sBuffer = shader->AsConstantBuffer("CB_Tessellation");

	
	vector< VertexTessellation> v;

	for (UINT i = 0; i < vertexCount; i++)
	{
		VertexTessellation tV=
		{
			vertices->Position
		};

		v.push_back(tV);
	}
	this->vertices = new VertexTessellation[v.size()];
	this->vertexCount = vertexCount;

	copy(v.begin(),v.end(),stdext::checked_array_iterator<VertexTessellation*>(this->vertices, this->vertexCount));

	this->indices = &indices;
	this->indexCount = indexCount;

	vertexBuffer = new VertexBuffer(this->vertices, this->vertexCount, sizeof(VertexTessellation));
	indexBuffer = new IndexBuffer(this->indices, this->indexCount);
}

Tessellation::~Tessellation()
{
	SafeDelete(sBuffer);

}

void Tessellation::Update()
{
	Super::Update();
}

void Tessellation::Render()
{

	Super::Render();

	buffer->Render();
	sBuffer->SetConstantBuffer(buffer->Buffer());

}


