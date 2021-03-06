#version 330 core

out vec4 fragColor;

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

in vec2 texCoord;
in vec3 fragPos;
in vec3 fragNormal;


uniform vec3 uViewPos;
uniform sampler2D uTexture;

#define NR_POINT_LIGHTS 1
#define NR_SPOT_LIGHTS 10
uniform DirLight uDirLight;
uniform PointLight uPointLights[NR_POINT_LIGHTS];
uniform SpotLight uSpotLights[NR_SPOT_LIGHTS];

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 norm = normalize(fragNormal);
	vec3 viewDir = normalize(uViewPos - fragPos);

	vec3 result = CalcDirLight(uDirLight, norm, viewDir);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(uPointLights[i], norm, fragPos, viewDir);

    for(int i = 0; i < NR_SPOT_LIGHTS; i++)
        result += CalcSpotLight(uSpotLights[i], norm, fragPos, viewDir);

    fragColor = vec4(result, 1.0f);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0f);
    
    vec3 ambient = light.ambient * vec3(texture(uTexture, texCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(uTexture, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(uTexture, texCoord));
    
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    float diff = max(dot(normal, lightDir), 0.0);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0f);
    
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    vec3 ambient = light.ambient * vec3(texture(uTexture, texCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(uTexture, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(uTexture, texCoord));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    float diff = max(dot(normal, lightDir), 0.0);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0f);
    
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    vec3 ambient = light.ambient * vec3(texture(uTexture, texCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(uTexture, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(uTexture, texCoord));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}