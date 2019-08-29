#version 330 core

out vec4 fragColor;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	//sampler2D emission;
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

	float theta = dot(lightDir,normalize(-light.direction));

	if(theta > light.cutOff)
	{

	    // calculating the attenuation
	    float distances = length(light.position - FragPos);
	    float attenuation = 1.0/(light.constant + light.linear * distances + light.quadratic *(distances * distances));
	    
	    // calculating ambient light; 
	    vec3 ambient =  light.ambient * vec3(texture(material.diffuse,TexCoords));
	    	ambient *= attenuation;
	    // calculating diffuse light; 
	    vec3 norm = normalize(Normal); 
	    
	    float diff = max(dot(norm,lightDir),0.0);
	    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));
	    diffuse *= attenuation;
	    
	    // calculating specular light; 
	    vec3 viewDir = normalize(viewPos - FragPos);
	    vec3 reflectDir = reflect(-lightDir,norm);
	    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	    vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoords));
	    specular *= attenuation;

	    vec3 result = (ambient + diffuse + specular ); //+ emission
        fragColor =  vec4(result,1.0f);

	} else{
	
		fragColor = vec4(light.ambient * texture(material.diffuse,TexCoords).rgb,1.0f);
	}



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