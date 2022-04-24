#include"Framework.h"
#include"MeshTerrain.h"

MeshTerrain::MeshTerrain(wstring heightFile)
{
	heightMap = new Texture(heightFile);

}

MeshTerrain::~MeshTerrain()
{

}


float MeshTerrain::GetHeight(Vector3& position)
{
	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	if (x < 0 || x > width) return FLT_MIN;
	if (z < 0 || z > height) return FLT_MIN;


	UINT index[4];
	index[0] = width * z + x;
	index[1] = width * (z + 1) + x;
	index[2] = width * z + x + 1;
	index[3] = width * (z + 1) + x + 1;

	Vector3 v[4];
	for (int i = 0; i < 4; i++)
		v[i] = vertices[index[i]].Position;


	float ddx = (position.x - v[0].x) / 1.0f;
	float ddz = (position.z - v[0].z) / 1.0f;

	Vector3 result;

	if (ddx + ddz <= 1.0f) //매끄럽게 표현
		result = v[0] + (v[2] - v[0]) * ddx + (v[1] - v[0]) * ddz;
	else
	{
		ddx = 1.0f - ddx;
		ddz = 1.0f - ddz;

		result = v[3] + (v[1] - v[3]) * ddx + (v[2] - v[3]) * ddz;
	}

	return result.y;
}

float MeshTerrain::GetVerticalRaycast(Vector3& position)
{
	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	if (x < 0 || x > width) return FLT_MIN;
	if (z < 0 || z > height) return FLT_MIN;


	UINT index[4];
	index[0] = width * z + x;
	index[1] = width * (z + 1) + x;
	index[2] = width * z + x + 1;
	index[3] = width * (z + 1) + x + 1;

	Vector3 p[4];
	for (int i = 0; i < 4; i++)
		p[i] = vertices[index[i]].Position;

	Vector3 start(position.x, 50.0f, position.z);
	Vector3 direction(0, -1, 0);

	float u, v, distance;
	Vector3 result(-1, FLT_MIN, -1);

	if (D3DXIntersectTri(&p[0], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
		result = p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;

	if (D3DXIntersectTri(&p[3], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
		result = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;

	return result.y;
}

Vector3 MeshTerrain::GetRaycastPosition()
{
	Matrix V = Context::Get()->View();
	Matrix P = Context::Get()->Projection();
	Viewport* vp = Context::Get()->GetViewport();

	Vector3 mouse = Mouse::Get()->GetPosition();


	Vector3 n, f;
	mouse.z = 0.0f;
	vp->Unproject(&n, mouse, world, V, P);

	mouse.z = 1.0f;
	vp->Unproject(&f, mouse, world, V, P);

	Vector3 start = n;
	Vector3 direction = f - n;


	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			UINT index[4];
			index[0] = width * z + x;
			index[1] = width * (z + 1) + x;
			index[2] = width * z + x + 1;
			index[3] = width * (z + 1) + x + 1;


			Vector3 p[4];
			for (int i = 0; i < 4; i++)
				p[i] = vertices[index[i]].Position;


			float u, v, distance;

			if (D3DXIntersectTri(&p[0], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
				return p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;

			if (D3DXIntersectTri(&p[3], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
				return p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;
		}
	}

	return Vector3(-1, FLT_MIN, -1); //지형 어느 곳에도 충돌 x
}

void MeshTerrain::Create()
{
	CreateVertexData();
	CreateIndexData();
	CreateNormalData();
}

void MeshTerrain::CreateVertexData()
{
	vector<Color> heights;
	heightMap->ReadPixel(DXGI_FORMAT_R8G8B8A8_UNORM, &heights);

	width = heightMap->GetWidth();
	height = heightMap->GetHeight();



	vector<MeshVertex> v;
	//매시 갯수
	UINT countX = 21;
	UINT countZ = 21;

	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			UINT index = width * z + x;
			UINT pixel = width * (height - 1 - z) + x;

			//vertices[index].Position.x = (float)x;
			//vertices[index].Position.y = heights[pixel].r * 255.0f / 10.0f;
			//vertices[index].Position.z = (float)z;


			MeshVertex vertex;
			vertex.Position = Vector3((float)x, heights[pixel].r * 255.0f/10.0f, (float)z);
			vertex.Normal = Vector3(0, 1, 0);
			vertex.Tangent = Vector3(0, 0, 0);
			vertex.Uv.x = (float)x *(float)(countX - 1 ) /width;
			vertex.Uv.y = (float)z *(float)(countZ - 1 ) /height;

			v.push_back(vertex);
		}
	}
	vertices = new MeshVertex[v.size()];
	vertexCount = v.size();

	copy(v.begin(), v.end(), stdext::checked_array_iterator<MeshVertex*>(vertices, vertexCount));
	//vertexCount = width * height;
	//vertices = new MeshVertex[vertexCount];
}

void MeshTerrain::CreateIndexData()
{

	//그리드 그리기



	indexCount = (width - 1) * (height - 1) * 6;
	indices = new UINT[indexCount];

	UINT index = 0;
	for (UINT y = 0; y < height - 1; y++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			indices[index + 0] = width * y + x;
			indices[index + 1] = width * (y + 1) + x;
			indices[index + 2] = width * y + x + 1;
			indices[index + 3] = width * y + x + 1;
			indices[index + 4] = width * (y + 1) + x;
			indices[index + 5] = width * (y + 1) + x + 1;

			
			index += 6;
		}
	}


	//indices = new UINT[i.size()];
	//indexCount = i.size();
	//
	//indexCount = (width - 1) * (height - 1) * 6;
	//indices = new UINT[indexCount];
}

void MeshTerrain::CreateNormalData()
{
	for (UINT i = 0; i < indexCount / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		MeshVertex v0 = vertices[index0];
		MeshVertex v1 = vertices[index1];
		MeshVertex v2 = vertices[index2];


		Vector3 a = v1.Position - v0.Position;
		Vector3 b = v2.Position - v0.Position;

		Vector3 normal;
		D3DXVec3Cross(&normal, &a, &b);

		vertices[index0].Normal += normal;
		vertices[index1].Normal += normal;
		vertices[index2].Normal += normal;

	}
	for (UINT i = 0; i < vertexCount; i++)
		D3DXVec3Normalize(&vertices[i].Normal, &vertices[i].Normal);
}
