#ifndef WIRE_WINDOW_INTERNAL_H
#define WIRE_WINDOW_INTERNAL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct wire_window_internal_t {
	GLFWwindow* gl_window;
} wire_window_internal;

#endif //WIRE_WINDOW_INTERNAL_H