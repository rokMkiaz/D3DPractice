#pragma once
#include "Systems/IExecute.h"

class GetHeightMeshDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Ready() override {}
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override {};
	virtual void ResizeScreen() override {}

private:
	void Mesh();
	void MakeTessellation();
	void Pass(UINT mesh, UINT model, UINT anim);


private:
	Shader* shader;

	MeshRender* terrain;
	Material* grid;

	Tessellation* tessellation;
	//Material* tessellationMaterial;

	vector<MeshRender*> meshes;
	vector<ModelRender*> models;
	vector<ModelAnimator*> animators;

};