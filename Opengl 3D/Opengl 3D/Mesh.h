#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "Shader.h"

struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};
struct TextureModel {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:

	/*  Mesh Data  */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<TextureModel> textures;
	unsigned int VAO;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureModel> textures);
	void Draw(Shader shader);

	~Mesh();

private:
	unsigned int VBO, EBO;
	/*  Functions    */
   // initializes all the buffer objects/arrays
	void setupMesh();
};

#endif // !MESH_H



