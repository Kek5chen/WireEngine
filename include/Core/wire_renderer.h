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
	std::vector<wire_model_base*> models;
public:
	wire_shader_manager *shader_manager;

	wire_renderer(wire_window *parent);
	void initialize();
	void add_model(wire_model_base *model);
	void next_frame();
	~wire_renderer();
};

#endif //WIRE_RENDERER_H
