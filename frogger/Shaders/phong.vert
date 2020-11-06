#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

out vec3 texCoord;
out vec3 fragPos;
out vec3 fragNormal;

uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProj;

void main()
{
	fragPos = (uWorld * vec4(inPos, 1.0f)).xyz;
	fragNormal = (uWorld * vec4(inNormal, 1.0f)).xyz;

	gl_Position = uProj * uView * vec4(fragPos, 1.0f);

	texCoord = inTexCoord;
}