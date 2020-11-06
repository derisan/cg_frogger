#version 330 core

in vec2 texCoord;
in vec3 fragPos;
in vec3 fragNormal;

out vec4 fragColor;

struct Light
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 uViewPos;
uniform Light light;
uniform sampler2D uTexture;

void main()
{
	// ambient
	vec3 ambient = light.ambient * texture(uTexture, texCoord).rgb;

	// diffuse
	vec3 norm = normalize(fragNormal);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * texture(uTexture, texCoord).rgb;

	// specular
	vec3 viewDir = normalize(uViewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 100.0f);
	vec3 specular = light.specular * spec * texture(uTexture, texCoord).rgb;

	vec3 phong = ambient + diffuse + specular;
	fragColor = vec4(phong, 1.0f);
}
