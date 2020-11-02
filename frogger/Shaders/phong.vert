#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;

out vec3 fragPos;
out vec3 normal;

uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProj;

void main()
{
	fragPos = vec3(uWorld * vec4(inPos, 1.0f));
	normal = inNormal;

	gl_Position = uProj * uView * vec4(fragPos, 1.0f);
}