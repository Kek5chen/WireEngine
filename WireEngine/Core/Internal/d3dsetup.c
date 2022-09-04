#include "d3dsetup.h"

#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxguid.lib")

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
	viewport.Width = (float)window->width;
	viewport.Height = (float)window->height;

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