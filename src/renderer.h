#pragma once
#include <GL/glew.h>

#define NO_ASSERT 0
#if NO_ASSERT == 0
#define ASSERT(f) if (!(f)) __debugbreak()
#else 
#define ASSERT(f) f
#endif

#ifdef DEBUG
#define GLCall(f) GLClearErrors();\
		f;\
		ASSERT(GLLogErrors(#f, __FILE__, __LINE__))
#else
#define GLCall(f) f
#endif

static void GLClearErrors();
bool GLLogErrors(const char* function, const char* file, int line);