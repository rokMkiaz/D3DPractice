#pragma once
#include "Framework.h"

//=======================================================================================

struct Vertex
{
	Vertex()
		: Position(0, 0, 0) {}

	Vector3 Position;
};

//=======================================================================================

struct VertexColor
{
	VertexColor()
		: Position(0, 0, 0)
		, Color(0, 0, 0, 1) {}

	VertexColor(float x, float y, float z, float r, float g, float b)
	{
		Position.x = x;
		Position.y = y;
		Position.z = z;

		Color.r = r;
		Color.g = g;
		Color.b = b;
		Color.a = 1.0f;
	}

	Vector3	Position;
	Color Color;
};

//=======================================================================================

struct VertexTexture
{
	VertexTexture()
		: Position(0, 0, 0)
		, Uv(0, 0) {}

	Vector3	Position;
	Vector2	Uv;
};

//=======================================================================================

struct VertexNormal
{
	VertexNormal()
		: Position(0, 0, 0)
		, Normal(0, 0, 0) {}

	VertexNormal(float x, float y, float z, float nx, float ny, float nz)
	{
		Position.x = x;
		Position.y = y;
		Position.z = z;

		Normal.x = nx;
		Normal.y = ny;
		Normal.z = nz;
	}

	Vector3 Position;
	Vector3 Normal;
};