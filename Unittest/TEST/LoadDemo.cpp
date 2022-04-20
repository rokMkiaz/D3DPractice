#include"stdafx.h"
#include"LoadDemo.h"

void LoadDemo::Initialize()
{
	Context::Get()->GetCamera()->Position(0, 0, -5.0f);

	//Texture
	{ 
		textureShader = new Shader(L"16_Texture.fx");

		vertices[0][0].Position = Vector3(- 1.5f, +0.5f, 0.0f);
		vertices[0][1].Position = Vector3(-0.5f, +0.5f, 0.0f);
		vertices[0][2].Position = Vector3(- 1.5f, -0.5f, 0.0f);
		vertices[0][3].Position = Vector3(-0.5f, -0.5f, 0.0f);

		vertices[0][0].Uv = Vector2(0, 1);
		vertices[0][1].Uv = Vector2(0, 0);
		vertices[0][2].Uv = Vector2(1, 1);
		vertices[0][3].Uv = Vector2(1, 0);

		vertexBuffer[0] = new VertexBuffer(vertices[0], 4, sizeof(VertexTexture));
		indexBuffer[0] = new IndexBuffer(indices, 6);
	}

	//Material
	{
		materialShader = new Shader(L"16_Texture.fx");

		vertices[1][0].Position = Vector3(-0.5f, -0.5f, 0.0f);
		vertices[1][1].Position = Vector3(-0.5f, +0.5f, 0.0f);
		vertices[1][2].Position = Vector3(+0.5f, -0.5f, 0.0f);
		vertices[1][3].Position = Vector3(+0.5f, +0.5f, 0.0f);

		vertices[1][0].Uv = Vector2(0, 1);
		vertices[1][1].Uv = Vector2(0, 0);
		vertices[1][2].Uv = Vector2(1, 1);
		vertices[1][3].Uv = Vector2(1, 0);

		vertexBuffer[1] = new VertexBuffer(vertices[1], 4, sizeof(VertexTexture));
		indexBuffer[1] = new IndexBuffer(indices, 6);
	}

	D3DXMatrixIdentity(&world);

}
void LoadDemo::Destroy()
{
	SafeDelete(textureShader);

	SafeDeleteArray(*vertexBuffer);
	SafeDeleteArray(*indexBuffer);


	//SafeRelease(srv);
	SafeDelete(texture);
}

void LoadDemo::Update()
{
	if (ImGui::Button("Open") == true)
	{
		function<void(wstring)> f = bind(&LoadDemo::LoadTexture1, this, placeholders::_1);

		D3DDesc desc = D3D::GetDesc();
		Path::OpenFileDialog(L"", Path::ImageFilter, L"../../_Textures/", f, desc.handle);
	}

	if (ImGui::Button("Open") == true)
	{
		function<void(wstring)> f = bind(&LoadDemo::LoadTexture2, this, placeholders::_1);

		D3DDesc desc = D3D::GetDesc();
		Path::OpenFileDialog(L"", Path::ImageFilter, L"../../_Textures/", f, desc.handle);
	}


}

void LoadDemo::Render()
{
	textureShader->AsMatrix("World")->SetMatrix(world);
	textureShader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	textureShader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());

	if (texture != NULL)
		textureShader->AsSRV("Map")->SetResource(texture->SRV());


	if (texture != NULL)
		materialShader->AsSRV("Map")->SetResource(texture->SRV());


	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	vertexBuffer[0]->Render();
	indexBuffer[0]->Render();

	vertexBuffer[1]->Render();
	indexBuffer[1]->Render();

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	textureShader->DrawIndexed(0, 0, 6);
	materialShader->DrawIndexed(0, 0, 6);
}

void LoadDemo::LoadTexture1(wstring file)
{
	SafeDelete(texture);

	texture = new Texture(file);
}

void LoadDemo::LoadTexture2(wstring file)
{
	SafeDelete(material);

	material = new Material(materialShader);
	material->DiffuseMap(file);
}
