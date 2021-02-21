#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>

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

//this is some code i'm working on

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

	/*initialize GLEW
	  it needs to be initialized after a window has been created*/
	if (glewInit() != GLEW_OK)
		std::cout << "glewInit() failed\n";

	std::cout << "OpenGl version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLEW version : " << glewGetString(GLEW_VERSION) << std::endl;

	float vertices[] =
	{
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
		-1.0f, -0.5f, 0, 1, 0, 1, //  9
		-0.5f, -0.5f, 0, 1, 0, 1, // 10
		 0.0f, -0.5f, 0, 1, 0, 1, // 11
		-1.0f, -1.0f, 0, 1, 0, 1, // 12
		-0.5f, -1.0f, 0, 1, 0, 1, // 13
	     0.0f, -1.0f, 0, 1, 0, 1, // 14

		 //botright
		 0.5f,  0.0f, 0, 0, 1, 1, // 15
		 1.0f,  0.0f, 0, 0, 1, 1, // 16
		 0.5f, -0.5f, 0, 0, 1, 1, // 17
		 1.0f, -0.5f, 0, 0, 1, 1, // 18
		 0.5f, -1.0f, 0, 0, 1, 1, // 19
		 1.0f, -1.0f, 0, 0, 1, 1, // 20

		 //topright
		 0.5f,  0.5f, 1, 0, 0.75f, 1, // 21
		 1.0f,  0.5f, 1, 0, 0.75f, 1, // 22
		 0.5f,  1.0f, 1, 0, 0.75f, 1, // 23
		 1.0f,  1.0f, 1, 0, 0.75f, 1, // 24
	};

	unsigned int indexes[] =
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

	unsigned int vbo; //vertex buffer object 
	glGenBuffers(1, &vbo); //create a buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo); //"select" a buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 25, vertices, GL_STATIC_DRAW); // "fill" the buffer with data ( or just mention its size )

	//define how the vertex members should be interpreted
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0); //the first two elements of a vertex represent the position argument	//glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(sizeof(float) * 3)); //the forth element of a vertex represents the state argument (active = 1, inactive = 0)
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((const void*)(sizeof(float) * 2)));

	//"activate" the atributes
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);


	unsigned int ibo; //index buffer objext
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3 * 2 * 8, indexes, GL_STATIC_DRAW);

	//Parse the shaders from files and create a program
	std::string vertexShader;
	std::string fragmentShader;

	std::string vertexShaderPath   = "res/shaders/vertex.shader";
	std::string fragmentShaderPath = "res/shaders/fragment.shader";

	std::cout << "VERTEX SHADER" << "\n";
	ParseFile(vertexShaderPath, vertexShader, true);

	std::cout << "FRAGMENT SHADER" << "\n";
	ParseFile(fragmentShaderPath, fragmentShader, true);

	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 3 * 2 * 8, GL_UNSIGNED_INT, nullptr);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}