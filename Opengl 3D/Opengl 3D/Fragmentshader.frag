#version 330 core

out vec4 fragColor;

in vec3 Normal; 
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{   
// calculating diffuse light; 
	vec3 norm = normalize(Normal); 
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = diff * lightColor;

	// calculating ambient light; 
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// calculating specular light; 

	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),32);
	vec3 specular = specularStrength * spec * lightColor;

	// adding the light and multiply with a object color
	vec3 result = (ambient + diffuse + specular) * objectColor;
    fragColor =  vec4(result,1.0f); ; 
}  