#include "renderer.h"
#include <iostream>

//Calls glGetError() until there are no more error flags
//It should always be called before GLLogErrors()
void GLClearErrors()
{
	while (glGetError() != GL_NO_ERROR);
}

//Prints all errors found so far
//It should always be called after GLClearErrors()
bool GLLogErrors(const char* function, const char* file, int line)
{
	bool noErrors = true;

	while (unsigned int error = glGetError())
	{
		std::cout << "[OpenGL error] (" << error << "): " << function << " " << file << " : " << line << "\n";
		noErrors = false;
	}

	return noErrors;
}