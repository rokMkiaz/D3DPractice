#include "Framework.h"
#include "Tessellation.h"


Tessellation::Tessellation(Shader* shader)
	:Renderer(shader) 
{
	Topology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);


	tessellationBuffer = new ConstantBuffer(&desc, sizeof(Desc));
	sBuffer = shader->AsConstantBuffer("CB_Tessellation");

	D3D::GetDC()->HSSetConstantBuffers(0, 1, tessellationBuffer->pBuffer());
	D3D::GetDC()->DSSetConstantBuffers(0, 1, tessellationBuffer->pBuffer());
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
	desc.TessellationVP = Context::Get()->View(); * Context::Get()->Projection();
	desc.TessellationFactor = 1.0f;

	tessellationBuffer->Render();
	sBuffer->SetConstantBuffer(tessellationBuffer->Buffer());

	if (vertexCount != vertices.size())
	{
		vertexCount = vertices.size();

		SafeDelete(vertexBuffer);
		vertexBuffer = new VertexBuffer(&vertices[0], vertices.size(), sizeof(VertexTessellation));
		
		UINT indexCount = 3;
		UINT indices[] = { 0,1,2 };
		indexBuffer = new IndexBuffer(indices,indexCount);
	

	}


	

	Super::Render();


}

void Tessellation::Add(Vector3& position, Vector4& color)
{
	VertexTessellation vertex =
	{
		position,color
	};
	vertices.push_back(vertex);

}


