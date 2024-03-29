#include "Texture.h"
#include "Error.h"



Texture::Texture()
	: Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
	glGenTextures(1, &this->ID);
}


void Texture::Generate(GLuint width, GLuint height, unsigned char * data)
{
	this->Width = width;
	this->Height = height;
	// Create Texture
	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
	// Set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);

	glGenerateMipmap(GL_TEXTURE_2D);
	// Unbind texture

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::LoadTexture(const GLchar *file, GLboolean alpha)
{
	if (alpha)
	{
		this->Internal_Format = GL_RGBA;
		this->Image_Format = GL_RGBA;
	}
	// Load image
	int width, height;
	unsigned char* image = SOIL_load_image(file, &width, &height, 0, this->Image_Format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	if (image != nullptr)
	{
		this->Generate(width, height, image);

	}
	else
	{
		fatelError("TEXTURE DATA NOT LOADED");
	}
	// Now generate texture
	// And finally free image data
	SOIL_free_image_data(image);
}

void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}
