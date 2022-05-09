#include "Framework.h"
#include "Tessellation.h"


Tessellation::Tessellation(Shader* shader, MeshVertex *vertices, UINT vertexCount)
	:Renderer(shader) 
{
	Topology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	this->vertices->position = vertices->Position;
	this->vertices->color = vertices->Normal;
	this->vertexCount = vertexCount;


	tessellationBuffer = new ConstantBuffer(&desc, sizeof(Desc));
	sBuffer = shader->AsConstantBuffer("CB_Tessellation");
	sDiffuseMap = shader->AsSRV("TSMap");

	//D3D::GetDC()->HSSetConstantBuffers(0, 1, tessellationBuffer->pBuffer());
	//D3D::GetDC()->DSSetConstantBuffers(0, 1, tessellationBuffer->pBuffer());
}

Tessellation::~Tessellation()
{
	SafeDelete(sBuffer);
	SafeDelete(tessellationBuffer);

	SafeDelete(vertices);

}

void Tessellation::Update()
{
	Super::Update();
}


void Tessellation::Render()
{
	desc.TessellationVP = Context::Get()->View() * Context::Get()->Projection();
	desc.TessellationFactor[0] = vertices[0].position[0];
	desc.TessellationFactor[1] = vertices[0].position[1];
	desc.TessellationFactor[2] = vertices[0].position[2];


	tessellationBuffer->Render();
	sBuffer->SetConstantBuffer(tessellationBuffer->Buffer());



	vertexBuffer = new VertexBuffer(&vertices[0], vertexCount, sizeof(VertexTessellation));


	if ( texture != NULL)
	{
		sDiffuseMap->SetResource(texture->SRV());
	}

	Super::Render();
	
	shader->Draw(0, Pass(), vertexCount);

}



void Tessellation::AddTexture(wstring file)
{
	texture = new Texture(file);
}


