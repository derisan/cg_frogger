#version 330 core

in vec3 texCoord
in vec3 fragPos;
in vec3 fragNormal;

out vec4 fragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform sampler2D uTexture;

void main()
{
	// ambient
	vec3 ambient = vec3(0.2f, 0.2f, 0.2f);

	// diffuse
	vec3 norm = normalize(fragNormal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;

	// specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = spec * lightColor;

	vec3 phong = (ambient + diffuse + specular);
	fragColor = texture(uTexture, texCoord) * vec4(phong, 1.0f);
}
