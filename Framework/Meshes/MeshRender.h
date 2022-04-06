#pragma once
#define MAX_MESH_INSTANCE 500 //ÃÖ´ë ÀÎ½ºÅÏ½Ì °¹¼ö

class MeshRender
{
public:
	MeshRender(Shader* shader, Mesh* mesh);
	~MeshRender();

	Mesh* GetMesh() { return mesh; }

	void Update();
	void Render();

	void Pass(UINT val) { mesh->Pass(val); }

	Transform* AddTransform();
	Transform* GetTransform(UINT index) { return transforms[index]; }
	void UpdateTransforms();

private:
	Mesh* mesh;

	vector<Transform*> transforms;//±×¸®´Â °¹¼ö
	Matrix worlds[MAX_MESH_INSTANCE];

	VertexBuffer* instanceBuffer;
};