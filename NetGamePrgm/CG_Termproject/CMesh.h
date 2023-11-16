#pragma once
class CMesh
{
public:

	CMesh();
	~CMesh();
private:


	GLuint VAO, VBO_pos, VBO_normal, VBO_uv;
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_uvs;
	std::vector<glm::vec3> m_normals;

public:
	GLint loadOBJ(const char* path,
		std::vector<glm::vec3>& out_vertices,
		std::vector<glm::vec2>& out_uvs,
		std::vector<glm::vec3>& out_normals);

	void loadmesh();
	void render();





};

