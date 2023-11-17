#include "std.h"
#include "CMesh.h"
#include "CTexture.h"
#include "CShader.h"

CTexture* texture_ = new CTexture;

CMesh::CMesh()
{
    m_vertices.clear();
    m_uvs.clear();
    m_normals.clear();

}

CMesh::~CMesh()
{
}

GLint CMesh::loadOBJ(const char* path,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::vec2>& out_uvs,
    std::vector<glm::vec3>& out_normals)
{
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices{ out_vertices };
    std::vector< glm::vec2 > temp_uvs{ out_uvs };
    std::vector< glm::vec3 > temp_normals{ out_normals };

    float sumX = 0.0, sumY = 0.0, sumZ = 0.0;
    float aveX, aveY, aveZ;
    float scaleX, scaleY, scaleZ;
    float minX = 0.0, minY = 0.0, minZ = 0.0;
    float maxX = 0.0, maxY = 0.0, maxZ = 0.0;
    float scaleAll;

    float sizeX, sizeY, sizeZ;

    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("Impossible to open the file !\n");
        return false;
    }
    while (1) {

        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // else : parse lineHeader
        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

            if (vertex.x < minX) minX = vertex.x;
            if (vertex.y < minY) minY = vertex.y;
            if (vertex.z < minZ) minZ = vertex.z;
            if (vertex.x > maxX) maxX = vertex.x;
            if (vertex.y > maxY) maxY = vertex.y;
            if (vertex.z > maxZ) maxZ = vertex.z;
            sumX += vertex.x;
            sumY += vertex.y;
            sumZ += vertex.z;



            temp_vertices.push_back(vertex);
        }
        //정점 텍스쳐 읽기
        else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            temp_uvs.push_back(uv);
        } //정점 노멀벡터 읽기 
        else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);

        }
        else if (strcmp(lineHeader, "f") == 0) {
            //std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            if (matches != 9) {
                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);

        }


        aveX = sumX / vertexIndices.size();
        aveY = sumY / vertexIndices.size();
        aveZ = sumZ / vertexIndices.size();
        scaleX = maxX - minX;
        scaleY = maxY - minY;
        scaleZ = maxZ - minZ;
    }
    //std::cout << "aveX: " << aveX << " aveY: " << aveY << " aveZ: " << aveZ << std::endl;

    //std::cout << "scaleX: " << scaleX << " scaleY: " << scaleY << " scaleZ: " << scaleZ << std::endl;

    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        glm::vec3 temp;
        unsigned int vertexIndex = vertexIndices[i];
        temp = temp_vertices[vertexIndex - 1];
        //temp = temp_vertices[vertexIndex];
        temp.x = temp.x - minX;
        temp.y = temp.y - minY;
        temp.z = temp.z - minZ;

        temp.x = ((temp.x * 2.0f) / scaleX) - 1.0f;
        temp.y = ((temp.y * 2.0f) / scaleY) - 1.0f;
        temp.z = ((temp.z * 2.0f) / scaleZ) - 1.0f;


        out_vertices.push_back(temp);
        m_vertices.push_back(temp);

        //glm::vec3 vertex = temp_vertices[vertexIndex - 1];
        //outvertex.push_back(vertex);
    }



    for (unsigned int i = 0; i < uvIndices.size(); i++) {
        unsigned int uvIndex = uvIndices[i];
        glm::vec2 vertex = temp_uvs[uvIndex - 1];
        out_uvs.push_back(vertex);
        m_uvs.push_back(vertex);

    }
    for (unsigned int i = 0; i < normalIndices.size(); i++) {
        unsigned int normalIndex = normalIndices[i];
        glm::vec3 vertex = temp_normals[normalIndex - 1];
        out_normals.push_back(vertex);
        m_normals.push_back(vertex);

    }


    return out_vertices.size();
}

void CMesh::loadmesh()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO_pos);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
    glBufferData((GL_ARRAY_BUFFER), m_vertices.size() * sizeof(glm::vec3), &m_vertices.front(), GL_STATIC_DRAW);
    //GLint pAttribute = glGetAttribLocation(shader->Get_shaderProgram(), "aPos");
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &VBO_normal);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normal);
    glBufferData(GL_ARRAY_BUFFER,m_normals.size() * sizeof(glm::vec3), &m_normals.front(), GL_STATIC_DRAW);
    //GLint nAttribute = glGetAttribLocation(shader->Get_shaderProgram(), "aNormal");
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &VBO_uv);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_uv);
    glBufferData(GL_ARRAY_BUFFER, m_uvs.size() * sizeof(glm::vec2), &m_uvs.front(), GL_STATIC_DRAW);
    //GLint cAttribute9 = glGetAttribLocation(shader->Get_shaderProgram(), "aTexCoord");
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(2);
}

void CMesh::render()
{
    int modeltrans_ = glGetUniformLocation(CShader::GetInstance()->Get_shaderProgram(), "modeltrans");
    unsigned int objColorLocation_ = glGetUniformLocation(CShader::GetInstance()->Get_shaderProgram(), "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색


    glm::mat4 matrix(1.0f);

    matrix = glm::translate(matrix, m_vec3Translate);

    matrix = glm::rotate(matrix, glm::radians(m_vec3Rotate.x), glm::vec3(1.0, 0.0, 0.0));
    matrix = glm::rotate(matrix, glm::radians(m_vec3Rotate.y), glm::vec3(0.0, 1.0, 0.0));
    matrix = glm::rotate(matrix, glm::radians(m_vec3Rotate.z), glm::vec3(0.0, 0.0, 1.0));


    matrix = glm::scale(matrix, m_vec3Scale);

    glUniformMatrix4fv(modeltrans_, 1, GL_FALSE, glm::value_ptr(matrix));
    glBindVertexArray(VAO);
    glUniform3f(objColorLocation_, 1.0, 1.0, 1.0);
    texture_->InitTexture("t_yellow.png", &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
}

/// <summary>
/// ////////////////////////////////////////////////////////////////////////////////
/// </summary>

CTank::CTank()
{
}

CTank::~CTank()
{
}

GLint CTank::loadOBJ(const char* path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals)
{
    GLint result = CMesh::loadOBJ(path, out_vertices, out_uvs, out_normals);
    return result;
}

void CTank::loadmesh()
{
    CMesh::loadmesh();
}

void CTank::render()
{
    int modeltrans_ = glGetUniformLocation(CShader::GetInstance()->Get_shaderProgram(), "modeltrans");
    unsigned int objColorLocation_ = glGetUniformLocation(CShader::GetInstance()->Get_shaderProgram(), "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색


    glm::mat4 matrix(1.0f);

    matrix = glm::translate(matrix, m_vec3Translate);

    matrix = glm::rotate(matrix, glm::radians(m_vec3Rotate.x), glm::vec3(1.0, 0.0, 0.0));
    matrix = glm::rotate(matrix, glm::radians(m_vec3Rotate.y), glm::vec3(0.0, 1.0, 0.0));
    matrix = glm::rotate(matrix, glm::radians(m_vec3Rotate.z), glm::vec3(0.0, 0.0, 1.0));


    matrix = glm::scale(matrix, m_vec3Scale);

    glUniformMatrix4fv(modeltrans_, 1, GL_FALSE, glm::value_ptr(matrix));
    glBindVertexArray(VAO);
    glUniform3f(objColorLocation_, 2.0, 2.0, 2.0);
    texture_->InitTexture("t_yellow.png", &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glm::mat4 head = glm::mat4(1.0);
    head = glm::rotate(head, glm::radians(m_vec3HeadRotate.x), glm::vec3(1.0, 0.0, 0.0));
    head = glm::rotate(head, glm::radians(m_vec3HeadRotate.y), glm::vec3(0.0, 1.0, 0.0));
    head = glm::rotate(head, glm::radians(m_vec3HeadRotate.z), glm::vec3(0.0, 0.0, 1.0));
    head = matrix * head;
    glUniformMatrix4fv(modeltrans_, 1, GL_FALSE, glm::value_ptr(head));
    //head
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 21000, m_vertices.size());
}
