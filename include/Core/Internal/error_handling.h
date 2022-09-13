#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#define ERR_GLFW_INIT 1
#define ERR_CREATE_OGL_WINDOW 2
#define ERR_GLEW_INIT 3
#define ERR_SHADER_COMPILE 4
#define ERR_SHADER_LINK 5

namespace logger {
	void throw_critical_error(int err_id, const char* message);
	void throw_warning(int err_id, const char* message);
	void throw_error(int err_id, const char* message);
	void throw_info(const char* message);
}

#endif //ERROR_HANDLING_H