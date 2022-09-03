#pragma once
#include <Windows.h>

typedef struct wire_window_t {
	const char* name;
	int			width;
	int			height;
	HWND		assigned_window;
} wire_window;

void create_window(wire_window* window);