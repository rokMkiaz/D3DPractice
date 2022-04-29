#include"stdafx.h"
#include"GetHeightMeshDemo.h"

void GetHeightMeshDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(24, 54, 0);
	Context::Get()->GetCamera()->Position(-31, 53, -2);

	shader = new Shader(L"75_Lighting.fxo");
	//shader = new Shader(L"113_Shadow.fxo");


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
	//tessellation->Update();
}

void GetHeightMeshDemo::PreRender()
{
	grid->Render();
	terrain->Render();
	//tessellation->Render();
}

void GetHeightMeshDemo::Render()
{
	Pass(0,1,2);
	grid->Render();
	terrain->Render();
	
	//tessellation->Render();
	
}

void GetHeightMeshDemo::Mesh()
{
	grid = new Material(shader);
	grid->DiffuseMap(L"Terrain/DarkDirt.png");

	Transform* transform = NULL;
	terrain = new MeshRender(shader, new MeshTerrain(L"Terrain/Graymap2.png"));
	transform = terrain->AddTransform();
	transform->Position(0, 0, 0);
	transform->Scale(1, 1, 1);



	//MakeTessellation();
	terrain->UpdateTransforms();

	meshes.push_back(terrain);
}

void GetHeightMeshDemo::MakeTessellation()
{
	tessellation = new Tessellation(shader, ((MeshTerrain*)terrain)->GetVerticesData(), ((MeshTerrain*)terrain)->GetVertexCount(),
		*((MeshTerrain*)terrain)->GetIndicesData(), ((MeshTerrain*)terrain)->GetIndexCount());

	
}

void GetHeightMeshDemo::Pass(UINT mesh, UINT model, UINT anim)
{
	
	//tessellation->Pass(0);
	for (MeshRender* temp : meshes)
		temp->Pass(0);

	for (ModelRender* temp : models)
		temp->Pass(model);

	for (ModelAnimator* temp : animators)
		temp->Pass(anim);
}

