#include "wire_window.h"

void message_loop()
{
	MSG msg;

	while (GetMessageA(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
}

LRESULT wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	wire_window* window;
	window = (wire_window*) GetWindowLongPtrA(hWnd, GWLP_USERDATA);
	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

char* get_window_class_name(wire_window* window)
{
	size_t	wnd_name_len;
	char*	wnd_class_name;
	char*	c;

	if (!window || !*window->name)
		return 0;
	wnd_name_len = strlen(window->name);
	wnd_class_name = malloc(wnd_name_len + 6);
	if (!wnd_class_name)
		return 0;
	strcpy_s(wnd_class_name + 5, wnd_name_len + 1, window->name);
	_strupr_s(wnd_class_name + 5, wnd_name_len + 1);
	memcpy(wnd_class_name, "WIRE_", 5);
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

	event = CreateEventA(0, 1, 0, window->name);
	if (!event)
		return;
	wnd_class.cbSize = sizeof(WNDCLASSEXA);
	wnd_class.style = 0;
	wnd_class.lpfnWndProc = wnd_proc;
	wnd_class.cbClsExtra = 0;
	wnd_class.cbWndExtra = 0;
	wnd_class.hInstance = GetCurrentProcess();
	wnd_class.hIcon = 0;
	wnd_class.hCursor = 0;
	wnd_class.hbrBackground = 0;
	wnd_class.lpszMenuName = 0;
	wnd_class.lpszClassName = get_window_class_name(window);
	wnd_class.hIconSm = 0;

	RegisterClassExA(&wnd_class);

	wnd_rect.left = 0;
	wnd_rect.top = 0;
	wnd_rect.right = window->width;
	wnd_rect.bottom = window->height;
	AdjustWindowRectEx(&wnd_rect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0);

	window->assigned_window = CreateWindowExA(0, 
		wnd_class.lpszClassName,
		window->name, 
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
		CW_USEDEFAULT, CW_USEDEFAULT,
		wnd_rect.right - wnd_rect.left,
		wnd_rect.bottom - wnd_rect.top,
		0, 0,
		GetCurrentProcess(), 
		0);
	if (!window->assigned_window || window->assigned_window == INVALID_HANDLE_VALUE) {
		window->assigned_window = 0;
		return;
	}

	SetWindowLongPtrA(window->assigned_window, GWLP_USERDATA, (long long)window);
	
	SetEvent(event);
	message_loop();
}

// do not free wire_window till win32 window quits
void create_window(wire_window* window)
{
	HANDLE event;

	event = CreateEventA(0, 1, 0, window->name);
	if (!event)
		return;
	_beginthread(create_window_i, 2000, window);
	WaitForSingleObject(event, INFINITE);
	CloseHandle(event);
}