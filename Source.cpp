#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC
#include <gl/glew.h>
#include <GLFW/glfw3.h>



GLFWwindow* init_gl()
{
	if (!glfwInit())
	{
		fprintf(stderr, "ERROR: Could not start GLFW3\n");
		exit(1);
	}

	GLFWwindow* window = glfwCreateWindow(640, 480, "Extended GL init", NULL, NULL);
	if (!window)
	{
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		exit(1);
	}

	glfwMakeContextCurrent(window);

	glewInit();

	return window;
}


GLuint shader_program;
GLint vertex_position_attribute;

void init_shaders()
{
	const char* vertex_shader =
		"#version 400\n"
		"in vec3 vp;"
		"void main() {"
		"  gl_Position = vec4(vp, 1.0);"
		"}";
	const char* fragment_shader =
		"#version 400\n"
		"void main() {"
		"  gl_FragColor = vec4(0.5, 0.0, 0.5, 1.0);"
		"}";

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	shader_program = glCreateProgram();
	glAttachShader(shader_program, fs);
	glAttachShader(shader_program, vs);
	glLinkProgram(shader_program);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	vertex_position_attribute = glGetAttribLocation(shader_program, "vp");
	glEnableVertexAttribArray(vertex_position_attribute);
}


GLuint vbo;

void init_buffers()
{
	float points[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);
}


void draw_scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader_program);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(vertex_position_attribute, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}


int main()
{
	GLFWwindow* window = init_gl();
	init_shaders();
	init_buffers();

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		draw_scene();

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}