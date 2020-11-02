#version 330 core

out vec4 fragColor;

uniform vec3 uObjectColor;

void main()
{
	fragColor = vec4(uObjectColor, 1.0f);
}