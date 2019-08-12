#version 330 core
struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


out vec4 fragColor;

in vec3 Normal; 
in vec3 FragPos;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{   

	// calculating ambient light; 
	vec3 ambient =  light.ambient * material.ambient;

	// calculating diffuse light; 
	vec3 norm = normalize(Normal); 
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// calculating specular light; 
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	// adding the light and multiply with a object color
	vec3 result = (ambient + diffuse + specular);
    fragColor =  vec4(result,1.0f); ; 
}  