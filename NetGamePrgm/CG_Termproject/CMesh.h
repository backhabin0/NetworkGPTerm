#pragma once
class CMesh
{
public:

	CMesh();
	~CMesh();
public:
	unsigned int texture;
	GLuint VAO, VBO_pos, VBO_normal, VBO_uv;
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_uvs;
	std::vector<glm::vec3> m_normals;
	float yaw; 
	float x_pos;
	float y_pos;
	float z_pos;
public:
	virtual GLint loadOBJ(const char* path,
		std::vector<glm::vec3>& out_vertices,
		std::vector<glm::vec2>& out_uvs,
		std::vector<glm::vec3>& out_normals);

	virtual void loadmesh();
	virtual void render();

public:
	glm::vec3& GetTranslate() { return m_vec3Translate; }
	glm::vec3& GetRotate() { return m_vec3Rotate; }
	glm::vec3& GetScale() { return m_vec3Scale; }

	void Move(glm::vec3 move) { m_vec3Translate += move; }
	void Rotate(glm::vec3 vecAngle) { m_vec3Rotate += vecAngle; }
	void SetRotateY(float y) { m_vec3Rotate.y = y; }

	void SetTranslateX(float x) { m_vec3Translate.x = x; }
	void SetTranslateY(float x) { m_vec3Translate.y = x; }
	void SetTranslateZ(float x) { m_vec3Translate.z = x; }
	void SetScale(glm::vec3 vec) { m_vec3Scale = vec; }

protected:
	glm::vec3 m_vec3Translate{ 0.0, 0.0, 0.0 };
	glm::vec3 m_vec3Rotate{ 0.0, 0.0, 0.0 };
	glm::vec3 m_vec3Scale{ 1.0, 1.0, 1.0 };


};

class CTank : CMesh
{
public:
	CTank();
	~CTank();

	glm::vec3 m_vec3HeadRotate{ 0.0, 0.0, 0.0 };
public:
	virtual GLint loadOBJ(const char* path,
		std::vector<glm::vec3>& out_vertices,
		std::vector<glm::vec2>& out_uvs,
		std::vector<glm::vec3>& out_normals);
	virtual void loadmesh();
	virtual void render();

	void Rotate(glm::vec3 vecAngle) { CMesh::Rotate(vecAngle); }
	void SetRotateY(float y) { CMesh::SetRotateY(y); };
	void SetTranslateX(float x) { CMesh::SetTranslateX(x); }
	void SetTranslateY(float x) { CMesh::SetTranslateY(x); }
	void SetTranslateZ(float x) { CMesh::SetTranslateZ(x); }
};
