#pragma once

namespace logger {
	void throw_critical_error(int err_id, const char* message);
	void throw_warning(int err_id, const char* message);
	void throw_error(int err_id, const char* message);
	void throw_info(const char* message);
}