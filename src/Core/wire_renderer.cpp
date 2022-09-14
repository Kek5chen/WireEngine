#include "wire_renderer.h"
#include "../Shaders/vertex_shader.h"
#include "../Shaders/fragment_shader.h"

wire_renderer::wire_renderer(wire_window *parent) {
	this->parent = parent;
	this->shader_manager = new wire_shader_manager();
}

void wire_renderer::initialize() {
	glClearColor(0, 0, 0, 0);

	glGenVertexArrays(1, &shader_manager->vertex_array_id);
	glBindVertexArray(shader_manager->vertex_array_id);

	bool success = shader_manager->add_program(vertex_shader_source, fragment_shader_source);

	if (!success)
		return;

	glfwSwapInterval(0);
}

void wire_renderer::next_frame() {
	for(auto model : models)
		model->draw();

	glfwSwapBuffers(((wire_window_internal *) parent)->gl_window);
	glClear(GL_COLOR_BUFFER_BIT);

	glfwPollEvents();
}

wire_renderer::~wire_renderer() {
	delete shader_manager;
}

void wire_renderer::add_model(wire_model_base* model)
{
	if (!model->shader_program)
		model->shader_program = shader_manager->get_default_program();
	models.push_back(model);
}
