#pragma once

#include "Internal/wire_window_internal.h"

#define WM_PRIMARY (-1)
#define WM_WINDOWED (0)

#define WK_ESCAPE GLFW_KEY_ESCAPE

class wire_window : private wire_window_internal {
public:
	const char*	name;
	int			width;
	int			height;
	int			fullscreen_monitor;

	int		get_key(int key);
	bool	create_window();
	void	next_frame();
	void	close_window();
	bool	should_close();
};

