#include"stdafx.h"
#include"GetHeightMeshDemo.h"

void GetHeightMeshDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(24, 54, 0);
	Context::Get()->GetCamera()->Position(-31, 53, -2);

	shader = new Shader(L"75_Lighting.fx");
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
}

void GetHeightMeshDemo::PreRender()
{
	grid->Render();
	terrain->Render();
}

void GetHeightMeshDemo::Render()
{
	Pass(0,1,2);
	grid->Render();
	terrain->Render();
	
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



	terrain->UpdateTransforms();
	meshes.push_back(terrain);
}

void GetHeightMeshDemo::Pass(UINT mesh, UINT model, UINT anim)
{
	for (MeshRender* temp : meshes)
		temp->Pass(mesh);

	for (ModelRender* temp : models)
		temp->Pass(model);

	for (ModelAnimator* temp : animators)
		temp->Pass(anim);
}

