#include "std.h"
#include "CMesh.h"

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
        //���� �ؽ��� �б�
        else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            temp_uvs.push_back(uv);
        } //���� ��ֺ��� �б� 
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
        //m_vertices.push_back(temp);

        //glm::vec3 vertex = temp_vertices[vertexIndex - 1];
        //outvertex.push_back(vertex);
    }



    for (unsigned int i = 0; i < uvIndices.size(); i++) {
        unsigned int uvIndex = uvIndices[i];
        glm::vec2 vertex = temp_uvs[uvIndex - 1];
        out_uvs.push_back(vertex);
        //m_uvs.push_back(vertex);

    }
    for (unsigned int i = 0; i < normalIndices.size(); i++) {
        unsigned int normalIndex = normalIndices[i];
        glm::vec3 vertex = temp_normals[normalIndex - 1];
        out_normals.push_back(vertex);
        //m_normals.push_back(vertex);

    }


    return out_vertices.size();
}

void CMesh::loadmesh()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO_pos);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
    glBufferData((GL_ARRAY_BUFFER), m_vertices.size() * sizeof(glm::vec3), &m_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &VBO_normal);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normal);
    glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(glm::vec3), &m_normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
}