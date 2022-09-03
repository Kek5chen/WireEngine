#include "wire_window.h"

#include <process.h>

void initialize_d3d(wire_window* window)
{
	DXGI_SWAP_CHAIN_DESC scd;
	
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = window->assigned_window;
	scd.SampleDesc.Count = 4;
	scd.Windowed = 1;

	D3D11CreateDeviceAndSwapChain(0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		0,
		0,
		0,
		D3D11_SDK_VERSION,
		&scd,
		&window->dx_swapchain,
		&window->d3d_dev,
		0,
		&window->d3d_devcontext);
}

void clear_d3d(wire_window* window)
{
	window->dx_swapchain->lpVtbl->Release(window->dx_swapchain);
	window->d3d_dev->lpVtbl->Release(window->d3d_dev);
	window->d3d_devcontext->lpVtbl->Release(window->d3d_devcontext);
}

void message_loop(void)
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
	window = (wire_window*) (long long) GetWindowLongPtrA(hWnd, GWLP_USERDATA);
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

	initialize_d3d(window);
	
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
	_beginthread((_beginthread_proc_type) create_window_i, 2000, window);
	WaitForSingleObject(event, INFINITE);
	CloseHandle(event);
}

void close_window(wire_window* window)
{
	clear_d3d(window);
	SendMessageA(window->assigned_window, WM_CLOSE, 0, 0);
}