#include "Help.h"
#include "GLFW.h"


GLFW::GLFW()
{
	if (!glfwInit())
		throw std::runtime_error("Could not initialize GLFW");

	// I cant log smt here. Some error here appearse
	//LOG_INFO("GLFW is initialized");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	glfwSwapInterval(1);
}

GLFW::~GLFW()
{
	glfwTerminate();
}