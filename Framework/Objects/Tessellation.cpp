#include "Framework.h"
#include "Tessellation.h"


Tessellation::Tessellation(Shader* shader, MeshVertex *vertices, UINT vertexCount)
	:Renderer(shader) 
{
	Topology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	for (UINT i = 0; vertices == nullptr; i++)
	{
		if (vertices == nullptr)break;
		VertexTessellation vertex
		{
			vertices[i].Position,vertices[i].Normal
		};
		this->vertices.push_back(vertex);
	}
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

	

}

void Tessellation::Update()
{
	Super::Update();
}


void Tessellation::Render()
{
	desc.TessellationVP = Context::Get()->View() * Context::Get()->Projection();

	for (vector<VertexTessellation>::iterator iter = vertices.begin(); iter != vertices.end() ; iter++)
	{
		desc.TessellationFactor[0] = iter->position[0];
		desc.TessellationFactor[1] = iter->position[1];
		desc.TessellationFactor[2] = iter->position[2];
	}

	tessellationBuffer->Render();
	sBuffer->SetConstantBuffer(tessellationBuffer->Buffer());

	vertexBuffer = new VertexBuffer(&vertices[0], vertices.size(), sizeof(VertexTessellation));



	Super::Render();
	
	if ( texture != NULL)
	{
		sDiffuseMap->SetResource(texture->SRV());
	}
	shader->DrawIndexed(0, Pass(), vertexCount);

}



void Tessellation::AddTexture(wstring file)
{
	texture = new Texture(file);
}


