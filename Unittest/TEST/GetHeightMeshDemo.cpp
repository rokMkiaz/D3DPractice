#include"stdafx.h"
#include"GetHeightMeshDemo.h"

void GetHeightMeshDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(24, 54, 0);
	Context::Get()->GetCamera()->Position(-31, 53, -2);

	//shader = new Shader(L"75_Lighting.fxo");
	shader = new Shader(L"114_Tessellation_Test.fx");


	Mesh();

}

void GetHeightMeshDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(terrain);
}

void GetHeightMeshDemo::Update()
{
	
	terrain->Update();

	tessellation->Update();

}

void GetHeightMeshDemo::PreRender()
{
	grid->Render();
	terrain->Render();

	tessellation->Render();
	//tessellationMaterial->Render();
}

void GetHeightMeshDemo::Render()
{
	Pass(0,1,2);
	grid->Render();
	terrain->Render();

	
	tessellation->Render();
	//tessellationMaterial->Render();

	//MakeTessellation();
	///tessellation->Render();
	///tessellationMaterial->Render();

}

void GetHeightMeshDemo::Mesh()
{
	grid = new Material(shader);
	grid->DiffuseMap(L"Terrain/DarkDirt.png");

	Transform* transform = NULL;
	terrain = new MeshRender(shader, new MeshTerrain(L"Terrain/Graymap2.png", 10, 10));
	transform = terrain->AddTransform();
	transform->Position(0, 0, 20);
	transform->Scale(1, 1, 1);



	terrain->UpdateTransforms();

	MakeTessellation();


	meshes.push_back(terrain);
}

void GetHeightMeshDemo::MakeTessellation()
{
	//tessellationMaterial = new Material(shader);
	//tessellationMaterial->DiffuseMap(L"Terrain/DarkDirt.png");


	tessellation = new Tessellation(shader, terrain->GetMesh()->GetVerticesData(), terrain->GetMesh()->GetVertexCount());


	tessellation->AddTexture(L"Terrain/DarkDirt.png");


	
	

}

void GetHeightMeshDemo::Pass(UINT mesh, UINT model, UINT anim)
{
	tessellation->Pass(3);
	for (MeshRender* temp : meshes)
	{
		temp->Pass(0);
	}
	for (ModelRender* temp : models)
		temp->Pass(model);

	for (ModelAnimator* temp : animators)
		temp->Pass(anim);
}

