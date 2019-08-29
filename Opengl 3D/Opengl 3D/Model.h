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

class Model
{
public:
public:
	/*  Functions   */
	std::vector<TextureModel> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	Model(const char *path);
	void Draw(Shader shader);
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
	unsigned int TextureFromFile(const char *path, const std::string &directory);
};


#endif // !MODEL_H


