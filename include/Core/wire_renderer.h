#ifndef WIRE_RENDERER_H
#define WIRE_RENDERER_H

#include <fstream>
#include <vector>
#include "GL/glew.h"
#include "wire_shader_manager.h"

class wire_window;

class wire_renderer {
private:
	wire_window *parent;
public:
	wire_shader_manager *shader_manager;

	wire_renderer(wire_window *parent);
	void initialize();
	void draw_triangle();
	void terminate();
	~wire_renderer();
};

#endif //WIRE_RENDERER_H
