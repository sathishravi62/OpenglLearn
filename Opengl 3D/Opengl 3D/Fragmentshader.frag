#version 330 core

out vec4 fragColor;

in vec3 color;
in vec2 texCoord; 

uniform sampler2D image;

void main()
{    
    fragColor =  texture(image,texCoord); 
}  