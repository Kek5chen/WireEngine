#include "wire_window.h"

#include "Internal/error_handling.h"

wire_window::wire_window() {
	this->title = "WireEngine Window";
	this->width = 1280;
	this->height = 720;
	this->fullscreen_monitor = WM_WINDOWED;
	this->renderer = new wire_renderer(this);
}

// calls base constructor first
wire_window::wire_window(const char *name, int width, int height, int fullscreen_monitor) : wire_window() {
	if (name)
		this->title = name;
	if (this->width)
		this->width = width;
	if (this->height)
		this->height = height;
	this->fullscreen_monitor = fullscreen_monitor;
}

int wire_window::get_key(int key) {
	return glfwGetKey(this->gl_window, key);
}

int wire_window::create_window() {
	if (!glfwInit()) {
		logger::throw_critical_error(ERR_GLFW_INIT, "Could not initialize GLFW");
		return 1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor *monitor;
	int monitor_count;
	if (!this->fullscreen_monitor)
		monitor = 0;
	else if (this->fullscreen_monitor == WM_PRIMARY)
		monitor = glfwGetPrimaryMonitor();
	else
		monitor = glfwGetMonitors(&monitor_count)[this->fullscreen_monitor];

	this->gl_window = glfwCreateWindow(this->width, this->height, this->title, monitor, 0);
	if (!this->gl_window) {
		logger::throw_critical_error(ERR_CREATE_OGL_WINDOW, "Could not create new OpenGL Window");
		return 2;
	}
	glfwMakeContextCurrent(this->gl_window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		logger::throw_critical_error(ERR_GLEW_INIT, "Could not initialize GLEW");
		return 3;
	}

	glfwSetInputMode(this->gl_window, GLFW_STICKY_KEYS, true);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	renderer->initialize();

	return 0;
}

void wire_window::close_window() {
	renderer->terminate();
	delete this->renderer;
}

bool wire_window::should_close() {
	return glfwWindowShouldClose(this->gl_window);
}


wire_window::~wire_window() {
	glfwTerminate();
}

wire_renderer *wire_window::get_renderer() {
	return this->renderer;
}

void wire_window::change_title(const char* new_title)
{
	this->title = new_title;
	glfwSetWindowTitle(this->gl_window, this->title);
}
