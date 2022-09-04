#pragma once
#include <d3d11.h>

#include "../wire_window.h"

typedef struct wire_window_internal_t {
	wire_window base;
	HWND		assigned_window;
	ID3D11Device* d3d_dev;
	ID3D11DeviceContext* d3d_devcontext;
	IDXGISwapChain* dx_swapchain;
	ID3D11RenderTargetView* d3d_backbuffer;
} wire_window_internal;
