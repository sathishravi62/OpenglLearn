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

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

// positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

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
	lightingShader.loadShaderFromFile("Vertexshader.vert", "MultipleLightFragmentshader.frag");
	
	Shader lampShader;
	lampShader.loadShaderFromFile("lampVs.vert", "lampFs.frag");

	// create Light cube
	Objects lightingObject;
	lightingObject.CreateObject();

	// create LampObject
	Objects lampObjects;
	lampObjects.CreateObject();

	//// create texture
	Texture ourTexture;
	ourTexture.LoadTexture("texture/cont.png", GL_TRUE);
	Texture specularTex;
	specularTex.LoadTexture("texture/contSpec.png", GL_TRUE);


	lightingShader.SetInteger("material.diffuse", 0,GL_TRUE);
	lightingShader.SetInteger("material.specular", 1, GL_TRUE);
	

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

		// be sure to activate shader when setting uniforms/drawing objects
		lightingShader.Use();
		lightingShader.SetVector3f("viewPos", camera.position);
		lightingShader.SetFloat("material.shininess", 32.0f);

		/*
		   Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		   the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		   by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		   by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
		*/

		//// bind textures on corresponding texture units
		//ourTexture.Bind();

		// directional light
		lightingShader.SetVector3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
		lightingShader.SetVector3f("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.SetVector3f("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		lightingShader.SetVector3f("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		lightingShader.SetVector3f("pointLights[0].position", pointLightPositions[0]);
		lightingShader.SetVector3f("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.SetVector3f("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.SetVector3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.SetFloat("pointLights[0].constant", 1.0f);
		lightingShader.SetFloat("pointLights[0].linear", 0.09);
		lightingShader.SetFloat("pointLights[0].quadratic", 0.032);
		// point light 2
		lightingShader.SetVector3f("pointLights[1].position", pointLightPositions[1]);
		lightingShader.SetVector3f("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.SetVector3f("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.SetVector3f("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.SetFloat("pointLights[1].constant", 1.0f);
		lightingShader.SetFloat("pointLights[1].linear", 0.09);
		lightingShader.SetFloat("pointLights[1].quadratic", 0.032);
		// point light 3
		lightingShader.SetVector3f("pointLights[2].position", pointLightPositions[2]);
		lightingShader.SetVector3f("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.SetVector3f("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.SetVector3f("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.SetFloat("pointLights[2].constant", 1.0f);
		lightingShader.SetFloat("pointLights[2].linear", 0.09);
		lightingShader.SetFloat("pointLights[2].quadratic", 0.032);
		// point light 4
		lightingShader.SetVector3f("pointLights[3].position", pointLightPositions[3]);
		lightingShader.SetVector3f("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.SetVector3f("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.SetVector3f("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.SetFloat("pointLights[3].constant", 1.0f);
		lightingShader.SetFloat("pointLights[3].linear", 0.09);
		lightingShader.SetFloat("pointLights[3].quadratic", 0.032);
		// spotLight
		lightingShader.SetVector3f("spotLight.position", camera.position);
		lightingShader.SetVector3f("spotLight.direction", camera.front);
		lightingShader.SetVector3f("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		lightingShader.SetVector3f("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		lightingShader.SetVector3f("spotLight.specular", 1.0f, 1.0f, 1.0f);
		lightingShader.SetFloat("spotLight.constant", 1.0f);
		lightingShader.SetFloat("spotLight.linear", 0.09);
		lightingShader.SetFloat("spotLight.quadratic", 0.032);
		lightingShader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		lightingShader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
		// Implemeting Projection matrix
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		lightingShader.SetMatrix4("p", projection, GL_FALSE);

		// camera/view transformation
		glm::mat4 view = camera.getViewMatrix();
		lightingShader.SetMatrix4("view", view, GL_FALSE);// passing the value to the model matrix in shader
		
		glm::mat4 model = glm::mat4(1.0f);
		lightingShader.SetMatrix4("model", model, GL_FALSE);
		
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ourTexture.ID);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularTex.ID);



		// draw lighting object
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShader.SetMatrix4("model", model);
			lightingObject.draw();
		}

		lampShader.Use();
		lampShader.SetMatrix4("p", projection, GL_FALSE);
		lampShader.SetMatrix4("view", view, GL_FALSE);

		for (int i = 0; i < 4; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			lampShader.SetMatrix4("model", model, GL_FALSE);
			lampObjects.draw();
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
