#include "CTexture.h"
#include "std.h"
#include "CMesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
CTexture::CTexture()
{
}

CTexture::~CTexture()
{
}

void CTexture::InitTexture(const char* filename, unsigned int* texture)
{
	glGenTextures(1, texture);
	stbi_set_flip_vertically_on_load(true);
	glBindTexture(GL_TEXTURE_2D, *texture);
	// 텍스처 wrapping/filtering 옵션 설정(현재 바인딩된 텍스처 객체에 대해)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 텍스처 로드 및 생성
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}
