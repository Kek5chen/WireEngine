#pragma once


#include <Windows.h>
#include <d3d11.h>

typedef struct wire_window_t {
	const char* name;
	int			width;
	int			height;
	HWND		assigned_window;
	ID3D11Device* d3d_dev;
	ID3D11DeviceContext* d3d_devcontext;
	IDXGISwapChain* dx_swapchain;
	ID3D11RenderTargetView* d3d_backbuffer;
} wire_window;

void create_window(wire_window* window);
void close_window(wire_window* window);
