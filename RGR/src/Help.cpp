#include "Help.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::stringstream ss;
		ss << "[OpenGL ERROR] (" << error << "): " << function << std::endl << file << ":" << line;
		throw std::runtime_error(ss.str());
		return false;
	}
	return true;
}
