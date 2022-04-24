#pragma once
#include "Framework.h"

struct ParticleData
{
	enum class BlendType
	{
		Opaque = 0, Additive, AlphaBlend
	} Type = BlendType::Opaque;


	bool bLoop = false;

	wstring TextureFile = L"";


	UINT MaxParticles = 100;

	float ReadyTime = 1.0f;
	float ReadyRandomTime = 0.0f;//랜덤을 위한 변수

	float StartVelocity = 1.0f;
	float EndVelocity = 1.0f;

	float MinHorizontalVelocity = 0.0f;//수평 진행값
	float MaxHorizontalVelocity = 0.0f;

	float MinVerticalVelocity = 0.0f;//수직 진행값 (Z)
	float MaxVerticalVelocity = 0.0f;


	Vector3 Gravity = Vector3(0, 0, 0);

	float ColorAmount = 1.0f;//색 강도
	Color MinColor = Color(1, 1, 1, 1);
	Color MaxColor = Color(1, 1, 1, 1);

	float MinRotateSpeed = 0.0f;
	float MaxRotateSpeed = 0.0f;

	float MinStartSize = 100.0f;
	float MaxStartSize = 100.0f;

	float MinEndSize = 100.0f;
	float MaxEndSize = 100.0f;
};