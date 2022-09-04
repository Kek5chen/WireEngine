#include "wire_window.h"

#include <process.h>
#include <d3dx11.h>
#include <d3dx10.h>

void initialize_d3d(wire_window* window)
{
	DXGI_SWAP_CHAIN_DESC scd;
	ID3D11Texture2D* pBackBuffer;
	HRESULT result;
	
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Height = window->height;
	scd.BufferDesc.Width = window->width;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = window->assigned_window;
	scd.SampleDesc.Count = 4;
	scd.Windowed = 1;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	result = D3D11CreateDeviceAndSwapChain(0,
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
	if (FAILED(result) || !window->dx_swapchain)
		return;

	window->dx_swapchain->lpVtbl->GetBuffer(window->dx_swapchain, 0, &IID_ID3D11Texture2D, (LPVOID*)&pBackBuffer);
	window->d3d_dev->lpVtbl->CreateRenderTargetView(window->d3d_dev, (ID3D11Resource*)pBackBuffer, 0, &window->d3d_backbuffer);
	pBackBuffer->lpVtbl->Release(pBackBuffer);
	window->d3d_devcontext->lpVtbl->OMSetRenderTargets(window->d3d_devcontext, 1, &window->d3d_backbuffer, 0);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 10;
	viewport.TopLeftY = 10;
	viewport.Width = (float) window->width;
	viewport.Height = (float) window->height;

	window->d3d_devcontext->lpVtbl->RSSetViewports(window->d3d_devcontext, 1, &viewport);
}

void render_frame(wire_window* window)
{
	D3DXCOLOR col;

	col.r = .4f;
	col.g = .4f;
	col.b = .1f;
	col.a = 1.0f;
	window->d3d_devcontext->lpVtbl->ClearRenderTargetView(window->d3d_devcontext, window->d3d_backbuffer, (const FLOAT*)&col);
	window->dx_swapchain->lpVtbl->Present(window->dx_swapchain, 0, 0);
}

void clear_d3d(wire_window* window)
{
	window->dx_swapchain->lpVtbl->SetFullscreenState(window->dx_swapchain, 0, 0);
	window->dx_swapchain->lpVtbl->Release(window->dx_swapchain);
	window->d3d_backbuffer->lpVtbl->Release(window->d3d_backbuffer);
	window->d3d_dev->lpVtbl->Release(window->d3d_dev);
	window->d3d_devcontext->lpVtbl->Release(window->d3d_devcontext);
}

void message_loop(wire_window* window)
{
	MSG msg;

	initialize_d3d(window);

	while (1) {
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
	wnd_class.style = CS_HREDRAW | CS_VREDRAW;
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
	AdjustWindowRect(&wnd_rect, WS_OVERLAPPEDWINDOW, 0);

	window->assigned_window = CreateWindowExA(0, 
		wnd_class.lpszClassName,
		window->name, 
		WS_OVERLAPPEDWINDOW, 
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
	ShowWindow(window->assigned_window, SW_SHOW);
	SetWindowLongPtrA(window->assigned_window, GWLP_USERDATA, (long long)window);
	
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
	clear_d3d(window);
	SendMessageA(window->assigned_window, WM_CLOSE, 0, 0);
}