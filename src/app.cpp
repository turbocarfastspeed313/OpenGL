#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>

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

//Calls glGetError() until there are no more error flags
//It should always be called before GLLogErrors()
static void GLClearErrors()
{
	while (glGetError() != GL_NO_ERROR);
}

//Prints all errors found so far
//It should always be called after GLClearErrors()
static bool GLLogErrors(const char* function, const char* file, int line)
{
	bool noErrors = true;

	while (unsigned int error = glGetError())
	{
		std::cout << "[OpenGL error] (" << error << "): " << function << " " << file << " : " << line << "\n";
		noErrors = false;
	}

	return noErrors;
}
//Reads from an std::ifstream into a string
static void ParseFile(std::string& path, std::string& out, bool printSourceToConsole = false)
{
	std::ifstream fin(path);
	std::string temp;
	while (fin)
	{
		std::getline(fin, temp);
		out += temp; //add the line to output
		out += "\n"; //newline after each line for proper indentation
		temp = ""; //reset the temp string
	}

	if (printSourceToConsole)
		std::cout << out << "\n";
}

//Compiles a shader given the source code
//Returns the shader's id
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type); //create a shader and store its id
	const char* src = source.c_str(); //get a C-type string from std::string
	glShaderSource(id, 1, &src, nullptr); //provide the source
	glCompileShader(id);

	//test for compilation errors of the shader
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result); //store the compile status in result
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); //get the length of the compilation error message
		char* message = (char*)alloca(length * sizeof(char)); //allocate memory on the stack of length
		glGetShaderInfoLog(id, length, &length, message); //store the error message into "message"

		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader." << std::endl
			     << message << std::endl;

		glDeleteShader(id); //the shader's no longer needed, delete;
		return 0;
	}
	return id;
}

//the parameters are the source code for the shaders as strings
//provide OpenGL with the source code for the shaders
//link and compile the shaders
//returns the program's id
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
	{
		std::cout << "glfwInit() failed\n";
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1000, 1000, "Hello, Friend", NULL, NULL);
	if (!window)
	{
		std::cout << "window creation failed\n";
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(2);

	/*initialize GLEW
	  it needs to be initialized after a window has been created*/
	if (glewInit() != GLEW_OK)
		std::cout << "glewInit() failed\n";

	std::cout << "OpenGl version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLEW version : " << glewGetString(GLEW_VERSION) << std::endl;

	float vertices[] =
	{
		 //atrib 1 - position ( 2 floats )
		 //atrib 2 - color ( 4 floats )

		 0.0f,  0.0f, 1, 0, 0, 1, // 0 (origin)

		 //topleft
		 0.0f,  1.0f, 1, 0, 0, 1, // 1
		-0.5f,  1.0f, 1, 0, 0, 1, // 2
		-1.0f,  1.0f, 1, 0, 0, 1, // 3
		-1.0f,  0.5f, 1, 0, 0, 1, // 4
		-0.5f,  0.5f, 1, 0, 0, 1, // 5
		 0.0f,  0.5f, 1, 0, 0, 1, // 6
		-1.0f,  0.0f, 1, 0, 0, 1, // 7
		-0.5f,  0.0f, 1, 0, 0, 1, // 8

		//botleft	  
		-1.0f, -0.5f, 1, 0, 0, 1, //  9
		-0.5f, -0.5f, 1, 0, 0, 1, // 10
		 0.0f, -0.5f, 1, 0, 0, 1, // 11
		-1.0f, -1.0f, 1, 0, 0, 1, // 12
		-0.5f, -1.0f, 1, 0, 0, 1, // 13
	     0.0f, -1.0f, 1, 0, 0, 1, // 14

		 //botright
		 0.5f,  0.0f, 0, 0, 1, 1, // 15
		 1.0f,  0.0f, 0, 0, 1, 1, // 16
		 0.5f, -0.5f, 0, 0, 1, 1, // 17
		 1.0f, -0.5f, 0, 0, 1, 1, // 18
		 0.5f, -1.0f, 0, 0, 1, 1, // 19
		 1.0f, -1.0f, 0, 0, 1, 1, // 20

		 //topright
		 0.5f,  0.5f, 0, 0, 1, 1, // 21
		 1.0f,  0.5f, 0, 0, 1, 1, // 22
		 0.5f,  1.0f, 0, 0, 1, 1, // 23
		 1.0f,  1.0f, 0, 0, 1, 1  // 24
	};

	unsigned int grid[] =
	{
		1, 6, 5,
		5, 1, 2,

		4, 5, 7,
		7, 8, 5,

		8, 0, 11,
		11, 8, 10,

		10, 12, 13,
		9, 12, 10,

		11, 14, 17,
		17, 19, 14,

		17, 15, 18,
		16, 18, 15,

		6, 21, 15,
		0, 6 , 15,

		21, 23, 22,
		22, 24, 23
	};

	unsigned int hexagone[] =
	{
		1, 6, 4,
		4, 6, 0,
		0, 4, 7,

		7,  9,  0,
		0, 11,  9,
		9, 11, 14,

		14, 11, 18,
		11,  0, 18,
		18, 16,  0,

		0, 16,  6,
		6, 16, 22,
		22, 1,  6
	};

	unsigned int vbo; //vertex buffer object 
	GLCall(glGenBuffers(1, &vbo)); //create a buffer
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo)); //"select" a buffer
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 25, vertices, GL_STATIC_DRAW)); // "fill" the buffer with data ( or just mention its size )

	//define how the vertex members should be interpreted
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0)); //the first two elements of a vertex represent the position argument	//glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(sizeof(float) * 3)); //the forth element of a vertex represents the state argument (active = 1, inactive = 0)
	GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((const void*)(sizeof(float) * 2))));

	//"activate" the atributes
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));


	//index buffers
	unsigned int grid_ibo; //index buffer object for grid
	GLCall(glGenBuffers(1, &grid_ibo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grid_ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3 * 2 * 8, grid, GL_STATIC_DRAW));

	//unsigned int hexagone_ibo; //index buffer objext for hexagone
	//GLCall(glGenBuffers(1, &hexagone_ibo));
	//GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hexagone_ibo));
	//GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3 * 3 * 4, hexagone, GL_STATIC_DRAW));

	//Parse the shaders from files and create a program
	std::string vertexShader;
	std::string fragmentShader;

	std::string vertexShaderPath   = "res/shaders/vertex.shader";
	std::string fragmentShaderPath = "res/shaders/fragment.shader";

	std::cout << "VERTEX SHADER" << "\n";
	GLCall(ParseFile(vertexShaderPath, vertexShader, true));
	std::cout << "\n\n";

	std::cout << "FRAGMENT SHADER" << "\n";
	GLCall(ParseFile(fragmentShaderPath, fragmentShader, true));

	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	GLCall(glUseProgram(shader));

	GLCall(int u_Color_location = glGetUniformLocation(shader, "u_Color"));
	GLCall(glUniform4f(u_Color_location, 0.2f, 0.3f, 0.8f, 1.0f));
  
	float r = 0.0f;
	float g = 0.0f;
	float increment = 0.05f;
	//Render loop until the user closes window
	while (!glfwWindowShouldClose(window))
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		GLCall(glUniform4f(u_Color_location, r, g, 0.8f, 1.0f));
		GLCall(glDrawElements(GL_TRIANGLES, 3 * 2 * 8, GL_UNSIGNED_INT, nullptr));

		if (r > 1.0f || g > 1.0f) 
				increment = -0.05f;
		else if (r < 0.0f || g < 0.0f) 
				increment = 0.05f;
		r += increment;
		g += increment;

		GLCall(glfwSwapBuffers(window)); // swap front and back buffers
		GLCall(glfwPollEvents()); //poll for and process events
	}

	GLCall(glDeleteProgram(shader));
	glfwTerminate();
	return 0;
}