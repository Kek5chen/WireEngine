#ifndef WIRE_SHADER_MANAGER_H
#define WIRE_SHADER_MANAGER_H

#include <vector>
#include "GL/glew.h"

struct wire_shader {
	GLuint id;
	GLenum type;
	const char* source;

	wire_shader(GLenum type, const char* shader_source);
	void initialize();
};

class wire_shader_manager {
public:
	std::vector<wire_shader*> shaders;
	GLuint vertex_array_id;
	GLuint program;
	GLuint vbo;

	bool add_shader(const char* shader_source, GLenum type);
	void remove_shader(wire_shader* shader);
	bool relink();
};

#endif //WIRE_SHADER_MANAGER_H
