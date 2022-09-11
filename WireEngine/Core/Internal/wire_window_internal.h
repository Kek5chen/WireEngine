#pragma once
#include <d3d11.h>

#include "../wire_window.h"

typedef struct wire_window_internal_t : wire_window {
	HWND					assigned_window;
	ID3D11Device*			dev;
	ID3D11DeviceContext*	devcontext;
	IDXGISwapChain*			swapchain;
	ID3D11RenderTargetView* backbuffer;
	ID3D11Buffer*			vertex_buffer;
	ID3D11VertexShader*		vertex_shader;
	ID3D11PixelShader*		pixel_shader;
	ID3D11InputLayout*		input_layout;
} wire_window_internal;
