#include "wire_renderer.h"
#include "wire_window.h"
#include "Shaders/vertex_shader.h"
#include "Shaders/fragment_shader.h"
#include "Models/objects.h"

wire_renderer::wire_renderer(wire_window *parent) {
	this->parent = parent;
	this->shader_manager = new wire_shader_manager();
}

static const vector3 g_tri_vertexes[] = {
		{-1.0f, -1.0f, 0.0f},
		{1.0f, -1.0f, 0.0f},
		{0.0f, 1.0f, 1.0f}
};
GLuint vertexbuffer;

void wire_renderer::initialize() {
	glClearColor(0, 0, 0, 0);

	glGenVertexArrays(1, &shader_manager->vertex_array_id);
	glBindVertexArray(shader_manager->vertex_array_id);

	shader_manager->program = glCreateProgram();
	bool success = shader_manager->add_shader(vertex_shader_source, GL_VERTEX_SHADER);
	success = success && shader_manager->add_shader(fragment_shader_source, GL_FRAGMENT_SHADER);
	if (success)
		shader_manager->relink();
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_tri_vertexes), g_tri_vertexes, GL_STATIC_DRAW);
}

void wire_renderer::draw_triangle() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader_manager->program);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
	);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
}

void wire_renderer::terminate() {
	delete shader_manager;
}

wire_renderer::~wire_renderer() {
	terminate();
}
