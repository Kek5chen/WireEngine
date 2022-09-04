#pragma once
#include <d3d11.h>

#include "../wire_window.h"

typedef struct wire_window_internal_t {
	wire_window				base;
	HWND					assigned_window;
	ID3D11Device*			dev;
	ID3D11DeviceContext*	devcontext;
	IDXGISwapChain*			swapchain;
	ID3D11RenderTargetView* backbuffer;
} wire_window_internal;
