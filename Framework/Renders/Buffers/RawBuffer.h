#pragma once

class RawBuffer : public CsResource //바이트 단위로 CS에서다루게 되는 버퍼
{
public:
	RawBuffer(void* inputData, UINT inputByte, UINT outputByte);
	~RawBuffer();

private:
	void CreateInput() override;
	void CreateSRV() override;

	void CreateOutput() override;
	void CreateUAV() override;

	void CreateResult() override;

public:
	void CopyToInput(void* data);
	void CopyFromOuput(void* data);


private:
	void* inputData;

	UINT inputByte;
	UINT outputByte;
};