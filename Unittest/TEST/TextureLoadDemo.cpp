#include"stdafx.h"
#include"TextureLoadDemo.h"

void TextureLoadDemo::Initialize()
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

	vertexBuffer = new VertexBuffer(vertices,4,sizeof(VertexTexture));
	indexBuffer = new IndexBuffer(indices,6);


	D3DXMatrixIdentity(&world);

}
void TextureLoadDemo::Destroy()
{
	SafeDelete(shader);

	SafeDelete(vertexBuffer);
	SafeDelete(indexBuffer);


	//SafeRelease(srv);
	SafeDelete(texture);
}

void TextureLoadDemo::Update()
{
	if (ImGui::Button("Open") == true)
	{
		function<void(wstring)> f = bind(&TextureLoadDemo::LoadTexture, this, placeholders::_1);

		D3DDesc desc = D3D::GetDesc();
		Path::OpenFileDialog(L"", Path::ImageFilter, L"../../_Textures/", f, desc.handle);
	}


}

void TextureLoadDemo::Render()
{
	shader->AsMatrix("World")->SetMatrix(world);
	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());

	if (texture != NULL)
		shader->AsSRV("Map")->SetResource(texture->SRV());


	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	vertexBuffer->Render();
	indexBuffer->Render();
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->DrawIndexed(0, 0, 6);
}

void TextureLoadDemo::LoadTexture(wstring file)
{
	SafeDelete(texture);

	texture = new Texture(file);
}
