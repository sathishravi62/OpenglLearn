#version 330 core
layout (location = 0) in vec3 vertex; 
layout (location = 1) in vec3 aColor; 
layout (location = 2) in vec2 aTexCoord; 

out vec3 color;
out vec2 texCoord; 

void main()
{
    gl_Position = vec4(vertex ,1.0); // projection * model *
	color = aColor;
	texCoord = aTexCoord;
}