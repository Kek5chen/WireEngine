#ifndef WIRE_WINDOW_INTERNAL_H
#define WIRE_WINDOW_INTERNAL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct wire_window_internal {
	GLFWwindow* gl_window;
	virtual ~wire_window_internal();
};

#endif //WIRE_WINDOW_INTERNAL_H