#ifndef TEXTURE_H
#define TEXTURE_H
#include<GL/glew.h>
#include<SOIL/SOIL.h>


class Texture
{
public:
	Texture();

	GLuint ID;
	// Texture image dimensions
	GLuint Width, Height; // Width and height of loaded image in pixels
	// Texture Format
	GLuint Internal_Format; // Format of texture object
	GLuint Image_Format; // Format of loaded image
	// Texture configuration
	GLuint Wrap_S; // Wrapping mode on S axis
	GLuint Wrap_T; // Wrapping mode on T axis
	GLuint Filter_Min; // Filtering mode if texture pixels < screen pixels
	GLuint Filter_Max; // Filtering mode if texture pixels > screen pixels
	void LoadTexture(const GLchar *file, GLboolean alpha);
	// Binds the texture as the current active GL_TEXTURE_2D texture object
	void Bind() const;

private:
	// Generates texture from image data
	void Generate(GLuint width, GLuint height, unsigned char* data);
};

#endif // !TEXTURE_H



