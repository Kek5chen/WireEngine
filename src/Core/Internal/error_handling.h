#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

namespace logger {
	void throw_critical_error(int err_id, const char* message);
	void throw_warning(int err_id, const char* message);
	void throw_error(int err_id, const char* message);
	void throw_info(const char* message);
}

#endif //ERROR_HANDLING_H