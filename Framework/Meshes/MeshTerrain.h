#pragma once

class MeshTerrain : public Mesh
{
public:
	MeshTerrain(wstring heightFile);
	~MeshTerrain();

	float GetHeight(Vector3& position);
	float GetVerticalRaycast(Vector3& position);
	Vector3 GetRaycastPosition();

	MeshVertex* GetVerticesData() { return vertices; }
	UINT GetVertexCount() { return vertexCount; }

	UINT* GetIndicesData() { return indices; }
	UINT GetIndexCount() { return indexCount; }


private:
	void Create() override;
	void CreateVertexData();
	void CreateIndexData();
	void CreateNormalData();


private:
	Texture* heightMap;

	UINT width, height;



	Matrix world;
};
