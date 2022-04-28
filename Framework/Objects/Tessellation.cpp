#include "Framework.h"
#include "Tessellation.h"


Tessellation::Tessellation(Shader* shader, VertexTextureNormalTangent* vertices, UINT vertexCount, UINT&indices, UINT indexCount)
	:Renderer(shader) //L"76_Test.fx"
{
	
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

	TessellationBuffer = new ConstantBuffer(this, sizeof(VertexTessellation));

}

Tessellation::~Tessellation()
{


}

void Tessellation::Update()
{
	Super::Update();
}

void Tessellation::Render()
{

	Super::Render();
	D3D::GetDC()->HSSetConstantBuffers(0,1,TessellationBuffer->pBuffer());
	D3D::GetDC()->DSSetConstantBuffers(0, 1, TessellationBuffer->pBuffer());


}


