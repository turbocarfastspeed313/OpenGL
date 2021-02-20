#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type); //create a shader and store its id
	const char* src = source.c_str(); //get a C-type string from std::string
	glShaderSource(id, 1, &src, nullptr); //provide the source
	glCompileShader(id);

	//test for compilation errors of the shader
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader." << std::endl
			      << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

//the parameters are the source code for the shaders (as strings)
//provides OpenGL with the source code for the shaders
//links and compiles the shaders
//returns an id for the created shader
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
	window = glfwCreateWindow(720, 720, "Hello, Friend", NULL, NULL);
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

	//an array that will act as the position vertex buffer 
	float vert[18] = 
	{
		-0.5f,  -0.5f, 255, 7, 0, 1,
		 0.0f,   0.7f, 0, 0, 0, 1,
		 0.5f,  -0.5f, 255, 7, 0, 1,
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer); //create a buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer); //"select" a buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18, vert, GL_STATIC_DRAW); // "fill" the buffer with data ( or just mention its size )

	//define how the vertex members should be interpreted
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0); //the first two elements of a vertex represent the position argument	//glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(sizeof(float) * 3)); //the forth element of a vertex represents the state argument (active = 1, inactive = 0)
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((const void*)(sizeof(float) * 3)));

	//"activate" the atributes
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//TODO : try to understand shader syntax
	std::string vertexShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 1) in vec4 color_in;\n"
		"out vec4 color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	color = color_in;"
		"	gl_Position = position;\n"
		"}\n";

	std::string fragmentShader =
		"#version 330 core\n"
		"\n"
		"in vec4 color;\n"
		"out vec4 out_color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	out_color = color;\n" // orange
		"}\n";

	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}