#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

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
	float vert[6] = {
		-0.5f,  -0.5f,
		 0.0f,   0.7f,
		 0.5f,  -0.5f,
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer); //create a buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer); //"select" a buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, vert, GL_STATIC_DRAW); // "fill" the buffer with data ( or just mention its size )

	//define how the vertex members should be interpreted
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); //the first two elements of a vertex represent the position argument	//glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(sizeof(float) * 3)); //the forth element of a vertex represents the state argument (active = 1, inactive = 0)

	//"activate" the atributes
	glEnableVertexAttribArray(0);

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