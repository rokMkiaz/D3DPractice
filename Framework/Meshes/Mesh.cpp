#include "Framework.h"
#include "Mesh.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
	SafeDelete(perFrame);

	SafeDelete(vertexBuffer);
	SafeDelete(indexBuffer);

	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);
}

void Mesh::SetShader(Shader* shader)
{
	this->shader = shader;

	SafeDelete(perFrame);
	perFrame = new PerFrame(shader);
}

void Mesh::Update()
{
<<<<<<< HEAD
	
=======
>>>>>>> 6a3dabf1d266e1424da246b3aee6e75efb055cb3
	perFrame->Update();
}

void Mesh::Render(UINT drawCount)
{
	if (vertexBuffer == NULL || indexBuffer == NULL)
	{
		Create();
<<<<<<< HEAD
=======

>>>>>>> 6a3dabf1d266e1424da246b3aee6e75efb055cb3

		vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(MeshVertex));
		indexBuffer = new IndexBuffer(indices, indexCount);
	}
	perFrame->Render();
	vertexBuffer->Render();
	indexBuffer->Render();

	


	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	shader->DrawIndexedInstanced(0, pass, indexCount, drawCount);
}

<<<<<<< HEAD
=======

>>>>>>> 6a3dabf1d266e1424da246b3aee6e75efb055cb3
