#include "wire_shader_manager.h"
#include "Internal/error_handling.h"
#include <algorithm>

wire_shader::wire_shader(GLenum type, const char *shader_source) {
	this->type = type;
	this->source = shader_source;
}

bool wire_shader::compile() {
	this->id = glCreateShader(this->type);
	glShaderSource(this->id, 1, &this->source, NULL);
	glCompileShader(this->id);
	glGetShaderiv(this->id, GL_COMPILE_STATUS, &this->status);

	if (this->status) {
		logger::throw_info("Shader compiled successfully");
		return true;
	}
	GLint log_length;
	glGetShaderiv(this->id, GL_INFO_LOG_LENGTH, &log_length);
	std::vector<GLchar> error_log(log_length);
	glGetShaderInfoLog(this->id, log_length, &log_length, &error_log[0]);
	logger::throw_error(ERR_SHADER_COMPILE, &error_log[0]);
	this->destroy();
	return false;
}

void wire_shader::destroy() {
	glDeleteShader(this->id);
}

wire_shader::~wire_shader() {
	this->destroy();
}




bool wire_shader_manager::add_shader(const char *shader_source, GLenum type, GLuint program) {
	if (!shader_source)
		return false;

	wire_shader* shader = new wire_shader(type, shader_source);
	if (!shader->compile())
		return false;
	glAttachShader(program, shader->id);
	return true;
}

bool wire_shader_manager::relink(GLuint program) {
	glLinkProgram(program);
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint log_length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
		std::vector<GLchar> error_log(log_length);
		glGetProgramInfoLog(program, log_length, &log_length, &error_log[0]);
		logger::throw_error(ERR_SHADER_LINK, "Shader relink failed!");
		logger::throw_error(ERR_SHADER_LINK, &error_log[0]);
		terminate();
		return false;
	}
		logger::throw_info("Successfully linked shader.");
	return true;
}

void wire_shader_manager::terminate() {
	glDeleteVertexArrays(1, &vertex_array_id);
	for (auto program : programs)
		glDeleteProgram(program);
}

wire_shader_manager::~wire_shader_manager() {
	terminate();
}

GLuint wire_shader_manager::get_default_program()
{
	return programs[0];
}

bool wire_shader_manager::add_program(const char *vertex_shader_source, const char *fragment_shader_source)
{
	GLuint program = glCreateProgram();
	if (!add_shader(vertex_shader_source, GL_VERTEX_SHADER, program))
		return false;
	if (!add_shader(fragment_shader_source, GL_FRAGMENT_SHADER, program))
		return false;
	if (!relink(program))
		return false;
	programs.push_back(program);
	return true;
}
