#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <vector>

enum CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};
const float YAW = -90.0f; // X axis
const float PITCH = 0.0f; // y axis
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


class Camera
{
public:
	// Camera Attribute
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	// Euler Angles
	float yaw;
	float pitch;

	// camera option
	float moveSpeed;
	float mouseSensitivity;
	float zoom;

	// Constructor with vectors
	Camera(glm::vec3 _position,glm::vec3 up,float _yaw,float _pitch);

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float _yaw, float _pitch);

	glm::mat4 getViewMatrix();

	void processKeyBoard(CameraMovement direction, float deltaTime);

	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	void ProcessMouseScroll(float yoffset);

	~Camera();

private:
	void updateCameraVectors();
};


#endif // !CAMERA_H


