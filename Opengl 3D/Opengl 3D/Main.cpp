#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/rotate_vector.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"
#include "Error.h"
#include "Camera.h"
#include "Objects.h"


void frameBuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow * _window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGl", NULL, NULL);
	if (_window == NULL)
	{
		fatelError("Fail to create window");
		return -1;
	}

	glfwMakeContextCurrent(_window);
	glfwSetFramebufferSizeCallback(_window, frameBuffer_size_callback);
	glfwSetCursorPosCallback(_window, mouse_callback);
	glfwSetScrollCallback(_window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_DEPTH_TEST);

	Shader lightingShader;
	lightingShader.loadShaderFromFile("Vertexshader.vert", "Fragmentshader.frag");
	
	Shader lampShader;
	lampShader.loadShaderFromFile("lampVs.vert", "lampFs.frag");

	// create Light cube
	Objects lightingObject;
	lightingObject.CreateObject();

	// create LampObject
	Objects lampObjects;
	lampObjects.CreateObject();

	//// create texture
	//Texture ourTexture;
	//ourTexture.LoadTexture("texture/container.jpg", GL_TRUE);

	while (!glfwWindowShouldClose(_window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input 
		processInput(_window);

		// render
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


		//// bind textures on corresponding texture units
		//ourTexture.Bind();

		// draw our first triangle
		lightingShader.Use();
		lightingShader.SetVector3f("objectColor", glm::vec3(1.0f, 0.5f, 0.31f), GL_FALSE);
		lightingShader.SetVector3f("lightColor", glm::vec3(1.0f, 1.0f, 1.0f), GL_FALSE);

		// Implemeting Projection matrix
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		lightingShader.SetMatrix4("p", projection, GL_FALSE);

		// camera/view transformation
		glm::mat4 view = camera.getViewMatrix();
		lightingShader.SetMatrix4("view", view, GL_FALSE);// passing the value to the model matrix in shader
		
		glm::mat4 model = glm::mat4(1.0f);
		lightingShader.SetMatrix4("model", model, GL_FALSE);
		
		// draw lighting object
		lightingObject.draw();

		lampShader.Use();
		lampShader.SetMatrix4("p", projection, GL_FALSE);
		lampShader.SetMatrix4("view", view, GL_FALSE);

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lampShader.SetMatrix4("model", model, GL_FALSE);

		lampObjects.draw();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(_window);
		glfwPollEvents();
		
	}
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------

	glfwTerminate();
	return 0;
}

void frameBuffer_size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyBoard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyBoard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyBoard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyBoard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
