#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/rotate_vector.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include "Error.h"
#include "Camera.h"
#include "Objects.h"
#include <filesystem>

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

	// build and compile shaders
   // -------------------------
	Shader ourShader;
	ourShader.loadShaderFromFile("ModelVertexShader.vert", "MultipleLightFragmentShader.frag");

	Shader lampShader;
	lampShader.loadShaderFromFile("lampVs.vert", "lampFs.frag");

	// load models
	// -----------
	Model ourModel("nanosuit/nanosuit.obj");
	Objects lightCube;
	lightCube.CreateObject();
	
	glm::vec3 pointLightPositions[] = {
	  glm::vec3(0.7f,  0.2f,  2.0f),
	  glm::vec3(2.3f, -3.3f, -4.0f),
	  glm::vec3(-4.0f,  2.0f, -12.0f),
	  glm::vec3(0.0f,  0.0f, -3.0f)
	};

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	ourShader.Use();
	ourShader.SetInteger("material.diffuse", 0);
	ourShader.SetInteger("material.specular", 1);

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
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		ourShader.Use();
		ourShader.SetVector3f("viewPos", camera.position);
		
       // specular lighting doesn't have full effect on this object's material
		ourShader.SetFloat("material.shininess", ourModel.modelmaterial.Shininess);

		// directional light
		ourShader.SetVector3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
		ourShader.SetVector3f("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		ourShader.SetVector3f("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		ourShader.SetVector3f("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		ourShader.SetVector3f("pointLights[0].position", pointLightPositions[0]);
		ourShader.SetVector3f("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		ourShader.SetVector3f("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		ourShader.SetVector3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		ourShader.SetFloat("pointLights[0].constant", 1.0f);
		ourShader.SetFloat("pointLights[0].linear", 0.09);
		ourShader.SetFloat("pointLights[0].quadratic", 0.032);
		// point light 2
		ourShader.SetVector3f("pointLights[1].position", pointLightPositions[1]);
		ourShader.SetVector3f("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		ourShader.SetVector3f("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		ourShader.SetVector3f("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		ourShader.SetFloat("pointLights[1].constant", 1.0f);
		ourShader.SetFloat("pointLights[1].linear", 0.09);
		ourShader.SetFloat("pointLights[1].quadratic", 0.032);	
		// point light 3
		ourShader.SetVector3f("pointLights[2].position", pointLightPositions[2]);
		ourShader.SetVector3f("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		ourShader.SetVector3f("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		ourShader.SetVector3f("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		ourShader.SetFloat("pointLights[2].constant", 1.0f);
		ourShader.SetFloat("pointLights[2].linear", 0.09);
		ourShader.SetFloat("pointLights[2].quadratic", 0.032);	
		// point light 4
		ourShader.SetVector3f("pointLights[3].position", pointLightPositions[3]);
		ourShader.SetVector3f("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		ourShader.SetVector3f("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		ourShader.SetVector3f("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		ourShader.SetFloat("pointLights[3].constant", 1.0f);
		ourShader.SetFloat("pointLights[3].linear", 0.09);
		ourShader.SetFloat("pointLights[3].quadratic", 0.032);		
		// spotLight
		ourShader.SetVector3f("spotLight.position", camera.position);
		ourShader.SetVector3f("spotLight.direction", camera.front);
		ourShader.SetVector3f("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		ourShader.SetVector3f("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		ourShader.SetVector3f("spotLight.specular", 1.0f, 1.0f, 1.0f);
		ourShader.SetFloat("spotLight.constant", 1.0f);
		ourShader.SetFloat("spotLight.linear", 0.09);
		ourShader.SetFloat("spotLight.quadratic", 0.032);
		ourShader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		ourShader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		ourShader.SetMatrix4("projection", projection);
		ourShader.SetMatrix4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
		ourShader.SetMatrix4("model", model);

		
		for (int i = 0; i < ourModel.textures_loaded.size(); i++)
		{
			if (ourModel.textures_loaded[i].type == "texture_diffuse")
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, ourModel.textures_loaded[i].id);
			} else if (ourModel.textures_loaded[i].type == "texture_specular")
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, ourModel.textures_loaded[i].id);
			}
		}
		ourModel.Draw(ourShader);
	
		lampShader.Use();
		lampShader.SetMatrix4("projection", projection);
		lampShader.SetMatrix4("view", view);
		
		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			lampShader.SetMatrix4("model", model);
			lightCube.draw();
		}

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
