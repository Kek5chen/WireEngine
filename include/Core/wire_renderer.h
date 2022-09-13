#ifndef WIRE_RENDERER_H
#define WIRE_RENDERER_H

#include <fstream>
#include <vector>
#include "GL/glew.h"
#include "wire_shader_manager.h"
#include "wire_window.h"
#include "Models/objects.h"

class wire_window;

class wire_renderer {
private:
	wire_window *parent;
public:
	wire_shader_manager *shader_manager;

	wire_renderer(wire_window *parent);
	void initialize();
	void draw_triangle(vertex v1, vertex v2, vertex v3);
	void next_frame();
	void terminate();
	~wire_renderer();
};

#endif //WIRE_RENDERER_H
