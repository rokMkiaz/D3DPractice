#include "Framework.h"
#include "Tessellation.h"


Tessellation::Tessellation(Shader* shader)
	:Renderer(shader) 
{
	Topology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);


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
	desc.TessellationFactor[0] = vertices[0].position[0];
	desc.TessellationFactor[1] = vertices[0].position[1];
	desc.TessellationFactor[2] = vertices[0].position[2];


	tessellationBuffer->Render();
	sBuffer->SetConstantBuffer(tessellationBuffer->Buffer());


	if (vertexCount != vertices.size())
	{
		vertexCount = vertices.size();

		SafeDelete(vertexBuffer);
		vertexBuffer = new VertexBuffer(&vertices[0], vertices.size(), sizeof(VertexTessellation));

	}


	Super::Render();
	sDiffuseMap->SetResource(texture->SRV());
	shader->Draw(0, Pass(), vertexCount);

}

void Tessellation::Add(Vector3& position, Vector4& color)
{
	VertexTessellation vertex =
	{
		position,color
	};
	vertices.push_back(vertex);

}

void Tessellation::AddTexture(wstring file)
{
	texture = new Texture(file);
}


