#pragma once
#include "Systems/IExecute.h"

class LoadDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Ready() override {}
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

private:
	void LoadTexture1(wstring file);
	void LoadTexture2(wstring file);
private:
	Shader* textureShader;
	Shader* materialShader;

	VertexTexture vertices[2][6];
	VertexBuffer* vertexBuffer[2];

	UINT indices[6] = { 0, 1, 2, 2, 1, 3 };
	IndexBuffer* indexBuffer[2];

	Matrix world;


	Texture* texture = NULL;
	Material* material = NULL;
	
};