#pragma once

namespace log {
	void throw_critical_error(int err_id, const char* message);
	void throw_warning(int err_id, const char* message);
	void throw_error(int err_id, const char* message);
	void throw_info(const char* message);
}