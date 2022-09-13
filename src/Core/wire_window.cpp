#include "wire_window.h"

#include "Internal/error_handling.h"

int wire_window::get_key(int key)
{
	return glfwGetKey(this->gl_window, key);
}

bool wire_window::create_window()
{
	if (!glfwInit()) {
		logger::throw_critical_error(0, "Could not initialize GLFW");
		return false;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* monitor;
	int monitor_count;
	if (!this->fullscreen_monitor)
		monitor = 0;
	else if (this->fullscreen_monitor == WM_PRIMARY)
		monitor = glfwGetPrimaryMonitor();
	else
		monitor = glfwGetMonitors(&monitor_count)[this->fullscreen_monitor];
	
	this->gl_window = glfwCreateWindow(this->width, this->height, this->name, monitor, 0);
	if (!this->gl_window) {
		logger::throw_critical_error(1, "Could not create new OpenGL Window");
		return false;
	}
	glfwMakeContextCurrent(this->gl_window);

	glfwSetInputMode(this->gl_window, GLFW_STICKY_KEYS, true);

	return true;
}

void wire_window::next_frame()
{
	glfwSwapBuffers(this->gl_window);
	glfwPollEvents();
}

void wire_window::close_window()
{
	glfwTerminate();
}

bool wire_window::should_close()
{
	return glfwWindowShouldClose(this->gl_window);
}
