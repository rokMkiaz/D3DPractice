#include"stdafx.h"
#include"Test.h"

void Test::Initialize()
{
	Context::Get()->GetCamera()->Position(0, 0, -5.0f);

	shader = new Shader(L"16_Texture.fx");

	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);
	vertices[2].Position = Vector3(+0.5f, -0.5f, 0.0f);
	vertices[3].Position = Vector3(+0.5f, +0.5f, 0.0f);

	vertices[0].Uv = Vector2(0, 1);
	vertices[1].Uv = Vector2(0, 0);
	vertices[2].Uv = Vector2(1, 1);
	vertices[3].Uv = Vector2(1, 0);

	//vertexBuffer = new VertexBuffer(vertices,4,0);
	//indexBuffer = new IndexBuffer(indices,6);
	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(VertexTexture) * 4;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}

	//Create Index Buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(UINT) * 6;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = indices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &indexBuffer));
	}

	D3DXMatrixIdentity(&world);

}
void Test::Destroy()
{
	SafeDelete(shader);

	//SafeDelete(vertexBuffer);
	//SafeDelete(indexBuffer);
	SafeRelease(vertexBuffer);
	SafeRelease(indexBuffer);

	//SafeRelease(srv);
	SafeDelete(texture);
}

void Test::Update()
{
	if (ImGui::Button("Open") == true)
	{
		function<void(wstring)> f = bind(&Test::LoadTexture, this, placeholders::_1);

		D3DDesc desc = D3D::GetDesc();
		Path::OpenFileDialog(L"", Path::ImageFilter, L"../../_Textures/", f, desc.handle);
	}


}

void Test::Render()
{
	shader->AsMatrix("World")->SetMatrix(world);
	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());

	if (texture != NULL)
		shader->AsSRV("Map")->SetResource(texture->SRV());


	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	//vertexBuffer->Render();
	//indexBuffer->Render();
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->DrawIndexed(0, 0, 6);
}

void Test::LoadTexture(wstring file)
{
	SafeDelete(texture);

	texture = new Texture(file);
}
