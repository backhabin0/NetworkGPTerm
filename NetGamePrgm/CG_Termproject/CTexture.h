#pragma once
class CTexture
{
private:
	CTexture();
	~CTexture();
public:
	void InitTexture(const char* filename, unsigned int* texture);
};

