#pragma once

class VertexBuffer : public iBuffer 
{
public:
	VertexBuffer(void* data, UINT count, UINT stride, UINT slot = 0, bool bCpuWrite = false, bool bGpuWrite = false);
	~VertexBuffer();

	UINT Count() { return count; }
	UINT Stride() { return stride; }
	ID3D11Buffer* Buffer() { return buffer; }

	void Render();

private:
	ID3D11Buffer* buffer;

	void* data; //정점정보 받아오는것
	UINT count;
	UINT stride;
	UINT slot;

	bool bCpuWrite;
	bool bGpuWrite;
};

