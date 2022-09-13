#include "wire_renderer.h"
#include "../Shaders/vertex_shader.h"
#include "../Shaders/fragment_shader.h"

wire_renderer::wire_renderer(wire_window *parent) {
	this->parent = parent;
	this->shader_manager = new wire_shader_manager();
}
GLuint triangle_buffer;

void wire_renderer::initialize() {
	glClearColor(0, 0, 0, 0);

	glGenVertexArrays(1, &shader_manager->vertex_array_id);
	glBindVertexArray(shader_manager->vertex_array_id);

	shader_manager->program = glCreateProgram();
	bool success = shader_manager->add_shader(vertex_shader_source, GL_VERTEX_SHADER);
	success = success && shader_manager->add_shader(fragment_shader_source, GL_FRAGMENT_SHADER);
	if (success)
		shader_manager->relink();

	glGenBuffers(1, &triangle_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, triangle_buffer);
	glUseProgram(shader_manager->program);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
}

void wire_renderer::draw_triangle(vertex v1, vertex v2, vertex v3) {
	glClear(GL_COLOR_BUFFER_BIT);

	vertex triangle[] = { v1, v2, v3 };
	glBindBuffer(GL_ARRAY_BUFFER, triangle_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * 3, triangle, GL_STATIC_DRAW);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void wire_renderer::next_frame() {
	glfwSwapBuffers(((wire_window_internal *) parent)->gl_window);
	glfwPollEvents();
}

void wire_renderer::terminate() {
	glDeleteBuffers(1, &triangle_buffer);
	delete shader_manager;
}

wire_renderer::~wire_renderer() {
	terminate();
}
