#pragma once
class CTexture
{
public:
	CTexture();
	~CTexture();
public:
	void InitTexture(const char* filename, unsigned int* texture);
};

