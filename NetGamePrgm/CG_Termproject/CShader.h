#pragma once
#include "std.h"
class CShader
{
public:
	CShader();
	~CShader();
private:
	GLuint shaderProgram; //���̴� ���α׷� �̸�
	GLuint vertexShader;//���ؽ� ���̴� ��ü
	GLuint fragmentShader;//�����׸�Ʈ ���̴� ��ü
	GLchar* vertexSource;
	GLchar* fragmentSource;

public:
	void make_vertexShaders();
	void make_fragmentShaders();
	void make_shaderProgram();
	char* filetobuf(const char* file);

	GLuint Get_shaderProgram() { return shaderProgram; }
};

