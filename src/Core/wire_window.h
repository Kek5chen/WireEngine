#ifndef WIRE_WINDOW_H
#define WIRE_WINDOW_H

#include "wire_renderer.h"
#include "Internal/wire_window_internal.h"

#define WM_PRIMARY (-1)
#define WM_WINDOWED (0)

#define WK_ESCAPE GLFW_KEY_ESCAPE

class wire_renderer;

class wire_window : private wire_window_internal {
public:
	const char*		name;
	int				width;
	int				height;
	int				fullscreen_monitor;
	wire_renderer* 	renderer;

	wire_window();
	wire_window(const char* name, int width, int height, int fullscreen_monitor = WM_WINDOWED);
	int		get_key(int key);
	int		create_window();
	void	close_window();
	bool	should_close();
	~wire_window();
};

#endif //WIRE_WINDOW_H