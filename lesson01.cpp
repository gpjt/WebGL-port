#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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
	GLint p_matrix_uniform;
	GLint mv_matrix_uniform;

};
struct shader_program_details shader_program;

void init_shaders()
{
	const char* vertex_shader =
		"#version 400\n"
		""
		"in vec3 aVertexPosition;"
		""
		"uniform mat4 uMVMatrix;"
		"uniform mat4 uPMatrix;"
		""
		"void main() {"
		"  gl_Position = uPMatrix * uMVMatrix * vec4(aVertexPosition, 1.0);"
		"}";
	const char* fragment_shader =
		"#version 400\n"
		"void main() {"
		"  gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);"
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

	shader_program.p_matrix_uniform = glGetUniformLocation(
		shader_program.program, "uPMatrix"
	);
	shader_program.mv_matrix_uniform = glGetUniformLocation(
		shader_program.program, "uMVMatrix"
	);
}


glm::mat4 mv_matrix;
glm::mat4 p_matrix;

void set_matrix_uniforms() 
{
	glUniformMatrix4fv(
		shader_program.p_matrix_uniform, 1, GL_FALSE, 
		glm::value_ptr(p_matrix)
	);
	glUniformMatrix4fv(
		shader_program.mv_matrix_uniform, 1, GL_FALSE, 
		glm::value_ptr(mv_matrix)
	);
}



struct vertex_buffer {
	GLuint buffer;
	int item_size;
	int num_items;
};

struct vertex_buffer triangle_vertex_buffer;
struct vertex_buffer square_vertex_buffer;

void init_buffers()
{
	float triangle_vertices[] = {
		 0.0,  1.0,  0.0,
		-1.0, -1.0,  0.0,
		 1.0, -1.0,  0.0
	};

	glGenBuffers(1, &triangle_vertex_buffer.buffer);
	glBindBuffer(GL_ARRAY_BUFFER, triangle_vertex_buffer.buffer);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), triangle_vertices, GL_STATIC_DRAW);
	triangle_vertex_buffer.item_size = 3;
	triangle_vertex_buffer.num_items = 3;

	float square_vertices[] = {
		 1.0,  1.0,  0.0,
		-1.0,  1.0,  0.0,
		 1.0, -1.0,  0.0,
		-1.0, -1.0,  0.0
	};
	glGenBuffers(1, &square_vertex_buffer.buffer);
	glBindBuffer(GL_ARRAY_BUFFER, square_vertex_buffer.buffer);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), square_vertices, GL_STATIC_DRAW);
	square_vertex_buffer.item_size = 3;
	square_vertex_buffer.num_items = 4;
}


void draw_scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader_program.program);

	p_matrix = glm::perspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);

	mv_matrix = glm::mat4();

	mv_matrix = glm::translate(mv_matrix, glm::vec3(-1.5f, 0.0f, -7.0f));
	glBindBuffer(GL_ARRAY_BUFFER, triangle_vertex_buffer.buffer);
	glVertexAttribPointer(
		shader_program.vertex_position_attribute, triangle_vertex_buffer.item_size, 
		GL_FLOAT, GL_FALSE, 0, NULL
	);
	set_matrix_uniforms();
	glDrawArrays(GL_TRIANGLES, 0, triangle_vertex_buffer.num_items);

	mv_matrix = glm::translate(mv_matrix, glm::vec3(3.0f, 0.0f, 0.0f));
	glBindBuffer(GL_ARRAY_BUFFER, square_vertex_buffer.buffer);
	glVertexAttribPointer(
		shader_program.vertex_position_attribute, square_vertex_buffer.item_size,
		GL_FLOAT, GL_FALSE, 0, NULL
	);
	set_matrix_uniforms();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, square_vertex_buffer.num_items);
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