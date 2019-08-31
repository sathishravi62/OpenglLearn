#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>

#include "Shader.h"
#include "Mesh.h"

struct Material {
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	glm::vec3 Ambient;
	float Shininess;
};

class Model
{
public:
public:
	/*  Functions   */
	std::vector<TextureModel> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	Model(const char *path);
	void Draw(Shader shader);
	Material modelmaterial;
private:
	/*  Model Data  */
	std::vector<Mesh> meshes;
	std::string directory;
	/*  Functions   */
	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<TextureModel> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
		std::string typeName);
	GLuint TextureFromFile(const char *path, const std::string &directory);
	Material loadMaterial(aiMaterial* mat);
};


#endif // !MODEL_H


