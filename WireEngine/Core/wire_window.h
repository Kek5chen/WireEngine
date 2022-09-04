#pragma once


#include <Windows.h>
#include <d3d11.h>

typedef struct wire_window_t {
	const char* name;
	int			width;
	int			height;
} wire_window;

wire_window* new_wire_window(const char* name, const int width, const int height);
void create_window(wire_window* window);
void close_window(wire_window* window);
