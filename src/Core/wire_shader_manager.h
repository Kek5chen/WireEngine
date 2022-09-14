#ifndef WIRE_SHADER_MANAGER_H
#define WIRE_SHADER_MANAGER_H

#include <vector>
#include "GL/glew.h"

struct wire_shader {
	GLuint id;
	GLenum type;
	const char* source;
	GLint status;

	wire_shader(GLenum type, const char* shader_source);
	bool compile();
	void destroy();
	~wire_shader();
};

class wire_shader_manager {
private:
	std::vector<GLuint> programs;
public:
	GLuint vertex_array_id;
	GLuint vbo;

	bool add_shader(const char* shader_source, GLenum type, GLuint program);
	bool add_program(const char* vertex_shader_source, const char* fragment_shader_source);
	GLuint get_default_program();
	bool relink(GLuint program);
	void terminate();
	~wire_shader_manager();
};

#endif //WIRE_SHADER_MANAGER_H
