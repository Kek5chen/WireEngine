#pragma once

#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")
#pragma comment (lib, "d3d11.lib")

#include <Windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

typedef struct wire_window_t {
	const char* name;
	int			width;
	int			height;
	HWND		assigned_window;
	ID3D11Device* d3d_dev;
	ID3D11DeviceContext* d3d_devcontext;
	IDXGISwapChain* dx_swapchain;
} wire_window;

void create_window(wire_window* window);
void close_window(wire_window* window);
