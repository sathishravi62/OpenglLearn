#version 330 core
layout (location = 0) in vec3 vertex;  
layout (location = 1) in vec2 aTexCoord; 

out vec2 texCoord; 

uniform mat4 model;
uniform mat4 p;

void main()
{
    gl_Position = p * model * vec4(vertex ,1.0); // projection * model *
	texCoord = aTexCoord;
}