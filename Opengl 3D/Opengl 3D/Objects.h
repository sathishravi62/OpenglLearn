#ifndef OBJECTS_H
#define OBJECTS_H
#include <GL/glew.h>
#include <GLM/glm.hpp>

class Objects
{
public:
	Objects();
	~Objects();

	void CreateObject();
	void draw();
private:
	unsigned int VBO, VAO;

};

#endif // !OBJECTS_H



