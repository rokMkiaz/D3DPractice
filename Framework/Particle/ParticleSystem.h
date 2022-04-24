#pragma once

class ParticleSystem : public Renderer
{
public:
	ParticleSystem(wstring file);
	~ParticleSystem();

	void Reset();
	void Add(Vector3& position);

public:
	void Update();

private:
	void MapVertices();
	void Activate();
	void Deactivate();

public:
	void Render();

	ParticleData& GetData() { return data; }
	void SetTexture(wstring file);


private:
	void ReadFile(wstring file);

private:
	struct VertexParticle
	{
		Vector3 Position;
		Vector2 Corner; //(-1 ~ +1)
		Vector3 Velocity;
		Vector4 Random; //x:주기, y:크기, z:회전, w:색상
		float Time;
	};

private:
	struct Desc
	{
		Color MinColor;
		Color MaxColor;

		Vector3 Gravity;
		float EndVelocity;

		Vector2 StartSize;
		Vector2 EndSize;

		Vector2 RotateSpeed;
		float ReadyTime;
		float ReadyRandomTime;

		float ColorAmount;
		float CurrentTime;
		float Padding[2];
	} desc;

private:
	ParticleData data;

	Texture* map = NULL;
	ID3DX11EffectShaderResourceVariable* sMap;

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;


	VertexParticle* vertices = NULL;
	UINT* indices = NULL;

	float currentTime = 0.0f;
	float lastAddTime = 0.0f; //마지막 입자 시간

	UINT leadCount = 0; //가장 앞서는 개수
	UINT gpuCount = 0; //leadCount를 따라가며 GPU에 복사할 개수
	UINT activeCount = 0; // GpuCount를 따라가며 활성화할 개수
	UINT deactiveCount = 0; //activecount를 따라가며 비활성화시킬 개수
};