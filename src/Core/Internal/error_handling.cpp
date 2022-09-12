#include "error_handling.h"

#include <cstdio>

#include <GLFW/glfw3.h>

namespace log {
	void throw_critical_error(int err_id, const char* message)
	{
		printf("[CRIT:%4i] %s", err_id, message);
		getchar();
		glfwTerminate();
	}

	void throw_warning(int err_id, const char* message)
	{
		printf("[WARN:%4i] %s", err_id, message);
	}

	void throw_error(int err_id, const char* message)
	{
		printf("[ERR:%4i] %s", err_id, message);
	}

	void throw_info(const char* message)
	{
		printf("[INFO] %s", message);
	}
}