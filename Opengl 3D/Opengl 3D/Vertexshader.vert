#version 330 core
layout (location = 0) in vec3 vertex;  
layout (location = 1) in vec3 aNormal; 
layout (location = 2) in vec2 aTexCoords;

out vec3 Normal; 
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 p;

void main()
{
    gl_Position = p * view * model * vec4(vertex ,1.0); // projection * model *
	FragPos = vec3(model * vec4(vertex ,1.0));
	Normal = mat3(transpose(inverse(model))) * aNormal;	
	TexCoords = aTexCoords;
}