#pragma once

class CShader
{
	DECLARE_SINGLETON(CShader)
private:
	CShader();
	~CShader();
private:
	GLuint shaderProgram; //세이더 프로그램 이름
	GLuint vertexShader;//버텍스 세이더 객체
	GLuint fragmentShader;//프래그먼트 세이더 객체
	GLchar* vertexSource;
	GLchar* fragmentSource;

public:
	void make_vertexShaders();
	void make_fragmentShaders();
	void make_shaderProgram();
	char* filetobuf(const char* file);

	GLuint Get_shaderProgram() { return shaderProgram; }
};

