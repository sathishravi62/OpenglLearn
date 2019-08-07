#include "Error.h"
#include <iostream>
#include <GLFW/glfw3.h>

void fatelError(std::string errorMsg)
{
	std::cout << errorMsg << std::endl;
	std::cout << "Enter any key to quit....." << std::endl;
	int i;
	std::cin >> i;

	glfwTerminate();
}
