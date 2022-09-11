#include "wire_window.h"

#include <process.h>

#include "Internal/d3dsetup.h"
#include "Internal/wire_window_internal.h"

wire_window* new_wire_window(const char* name, const int width, const int height)
{
	auto window = new wire_window_internal();
	if (!window)
		return 0;
	window->name = name;
	window->width = width;
	window->height = height;
	return window;
}

void message_loop(wire_window* window)
{
	MSG msg;

	initialize_d3d(window);

	while (true) {
		if (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
		render_frame(window);
	}
	clear_d3d(window);
}

LRESULT wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	wire_window* window;
	window = (wire_window*) GetWindowLongPtrA(hWnd, GWLP_USERDATA);
	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

char* get_window_class_name(const wire_window* window)
{
	size_t	wnd_name_len;
	char*	wnd_class_name;
	char*	c;

	if (!window || !*window->name)
		return 0;
	wnd_name_len = strlen(window->name);
	wnd_class_name = new char[wnd_name_len + 6];
	if (!wnd_class_name)
		return 0;
	strcpy_s(wnd_class_name + 5, wnd_name_len + 1, window->name);
	_strupr_s(wnd_class_name + 5, wnd_name_len + 1);
	memcpy_s(wnd_class_name, wnd_name_len + 6, "WIRE_", 5);  // NOLINT(bugprone-not-null-terminated-result) <- we want this since we have text afterwards
	c = strchr(wnd_class_name, ' ');
	while (c) {
		*c = '_';
		c = strchr(wnd_class_name, ' ');
	}
	
	return wnd_class_name;
}


void create_window_i(wire_window* window)
{
	HANDLE		event;
	WNDCLASSEXA wnd_class;
	RECT		wnd_rect;
	wire_window_internal* wnd;

	wnd = (wire_window_internal*) window;
	event = CreateEventA(0, 1, 0, window->name);
	if (!event)
		return;
	wnd_class.cbSize = sizeof(WNDCLASSEXA);
	wnd_class.style = CS_HREDRAW | CS_VREDRAW;
	wnd_class.lpfnWndProc = wnd_proc;
	wnd_class.cbClsExtra = 0;
	wnd_class.cbWndExtra = 0;
	wnd_class.hInstance = (HINSTANCE) GetCurrentProcess();
	wnd_class.hIcon = 0;
	wnd_class.hCursor = 0;
	wnd_class.hbrBackground = 0;
	wnd_class.lpszMenuName = 0;
	wnd_class.lpszClassName = get_window_class_name(window);
	wnd_class.hIconSm = 0;

	RegisterClassExA(&wnd_class);

	wnd_rect.left = 0;
	wnd_rect.top = 0;
	wnd_rect.right = wnd->width;
	wnd_rect.bottom = wnd->height;
	AdjustWindowRect(&wnd_rect, WS_OVERLAPPEDWINDOW, 0);

	wnd->assigned_window = CreateWindowExA(0, 
		wnd_class.lpszClassName,
		window->name, 
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT,
		wnd_rect.right - wnd_rect.left,
		wnd_rect.bottom - wnd_rect.top,
		0, 0,
		(HINSTANCE) GetCurrentProcess(), 
		0);
	if (!wnd->assigned_window || wnd->assigned_window == INVALID_HANDLE_VALUE) {
		wnd->assigned_window = 0;
		return;
	}
	ShowWindow(wnd->assigned_window, SW_SHOW);
	SetWindowLongPtrA(wnd->assigned_window, GWLP_USERDATA, (long long)wnd);
	
	SetEvent(event);
	message_loop(window);
}

// do not free wire_window till win32 window quits
void create_window(wire_window* window)
{
	HANDLE event;

	event = CreateEventA(0, 1, 0, window->name);
	if (!event)
		return;
	_beginthread((_beginthread_proc_type) create_window_i, 2000, window);
	WaitForSingleObject(event, INFINITE);
	CloseHandle(event);
}

void close_window(wire_window* window)
{
	wire_window_internal* wnd;

	wnd = (wire_window_internal*) window;
	clear_d3d(window);
	SendMessageA(wnd->assigned_window, WM_CLOSE, 0, 0);
}