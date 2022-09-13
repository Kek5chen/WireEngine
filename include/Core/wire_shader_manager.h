#ifndef WIRE_SHADER_MANAGER_H
#define WIRE_SHADER_MANAGER_H

#include <vector>
#include "GL/glew.h"

struct wire_shader {
	GLuint id;
	GLenum type;
	const char* source;
	GLint status;
	bool linked;

	wire_shader(GLenum type, const char* shader_source);
	bool compile();
	void destroy();
	~wire_shader();
};

class wire_shader_manager {
public:
	std::vector<wire_shader*> shaders;
	GLuint vertex_array_id;
	GLuint program;
	GLuint vbo;

	bool add_shader(const char* shader_source, GLenum type);
	void clean_shaders();
	bool relink();
	void terminate();
	~wire_shader_manager();
};

#endif //WIRE_SHADER_MANAGER_H
