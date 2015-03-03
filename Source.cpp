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


struct shader_program_details {
	GLuint program;
	GLint vertex_position_attribute;
};
struct shader_program_details shader_program;

void init_shaders()
{
	const char* vertex_shader =
		"#version 400\n"
		"in vec3 aVertexPosition;"
		"void main() {"
		"  gl_Position = vec4(aVertexPosition, 1.0);"
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

	shader_program.program = glCreateProgram();
	glAttachShader(shader_program.program, fs);
	glAttachShader(shader_program.program, vs);
	glLinkProgram(shader_program.program);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	shader_program.vertex_position_attribute = glGetAttribLocation(
		shader_program.program, "aVertexPosition"
	);
	glEnableVertexAttribArray(shader_program.vertex_position_attribute);
}



struct vertex_buffer {
	GLuint buffer;
	int item_size;
	int num_items;
};

struct vertex_buffer triangle_vertex_buffer;

void init_buffers()
{
	float points[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	glGenBuffers(1, &triangle_vertex_buffer.buffer);
	glBindBuffer(GL_ARRAY_BUFFER, triangle_vertex_buffer.buffer);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);
	triangle_vertex_buffer.item_size = 3;
	triangle_vertex_buffer.num_items = 3;
}


void draw_scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader_program.program);

	glBindBuffer(GL_ARRAY_BUFFER, triangle_vertex_buffer.buffer);
	glVertexAttribPointer(
		shader_program.vertex_position_attribute, triangle_vertex_buffer.item_size, 
		GL_FLOAT, GL_FALSE, 0, NULL
	);
	glDrawArrays(GL_TRIANGLES, 0, triangle_vertex_buffer.num_items);
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