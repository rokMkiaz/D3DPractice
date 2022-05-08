#pragma once

class MeshTerrain : public Mesh
{
public:
	MeshTerrain(wstring heightFile,UINT countX,UINT countZ );
	~MeshTerrain();

	float GetHeight(Vector3& position);
	float GetVerticalRaycast(Vector3& position);
	Vector3 GetRaycastPosition();



private:
	void Create() override;
	void CreateVertexData();
	void CreateIndexData();
	void CreateNormalData();


private:
	Texture* heightMap;

	UINT width = 0, height = 0;

	UINT countX  = 0, countZ = 0;

	Matrix world;
};
