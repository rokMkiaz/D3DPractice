#pragma once

class RawBuffer : public CsResource //����Ʈ ������ CS�����ٷ�� �Ǵ� ����
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