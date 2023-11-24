#version 330 core

layout(location = 0) in vec3 aPos;   // ��ġ ������ attribute position 0�� �����ϴ�.
layout(location = 1)in vec3 aNormal;
uniform mat4 modeltrans;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

out vec3 FragPos;
out vec3 Normal;

void main()
{
	gl_Position = projectionTransform * viewTransform * modeltrans * vec4(aPos, 1.0);
	FragPos = vec3(modeltrans * vec4(aPos, 1.0));
	Normal = aNormal;
}