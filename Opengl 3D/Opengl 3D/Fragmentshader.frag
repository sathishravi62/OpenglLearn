#version 330 core

out vec4 fragColor;

struct Material
{
	sampler2D texture_diffuse;
	sampler2D texture_specular;
	float shininess;
};

struct Light {
    vec3 position;
	vec3 direction; // Spot Light
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
	float linear;
	float quadratic;
	float cutOff; // Spot Light
	float outerCutOff; // Spot Light
};



in vec3 Normal; 
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform float time;

void main()
{   
	vec3 lightDir = normalize(light.position - FragPos);

	

	
	// calculating ambient light; 
	vec3 ambient =  light.ambient * vec3(texture(material.diffuse,TexCoords));
		
	// calculating diffuse light; 
	vec3 norm = normalize(Normal); 
	
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));
	
	
	// calculating specular light; 
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoords));
	

	float theta = dot(lightDir,normalize(-light.direction));
	float epslion = (light.cutOff - light.outerCutOff);
	float intensity = clamp((theta - light.outerCutOff) / epslion,0.0,1.0);
	diffuse *= intensity;
	specular *= intensity;

	// calculating the attenuation
	float distances = length(light.position - FragPos);
	float attenuation = 1.0/(light.constant + light.linear * distances + light.quadratic *(distances * distances));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = (ambient + diffuse + specular ); //+ emission
    fragColor =  vec4(result,1.0f);

	


	// adding emission
//	vec3 emission = vec3(0.0);
//	if (texture(material.specular, TexCoords).r == 0.0)   /*rough check for blackbox inside spec texture */
//    {
//        /*apply emission texture */
//        emission = texture(material.emission, TexCoords).rbg;
//        
//        /*some extra fun stuff with "time uniform" */
//        emission = texture(material.emission, TexCoords + vec2(0.0,time)).rgb;   /*moving */
//        emission = emission * (sin(time) * 0.5 + 0.5) * 2.0;                     /*fading */
//
//    } 
	// adding the light and multiply with a object color
}  