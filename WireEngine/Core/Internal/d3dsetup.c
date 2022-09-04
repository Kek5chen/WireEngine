#include "d3dsetup.h"

#include "wire_window_internal.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxguid.lib")

#include <d3dx10.h>

void initialize_d3d(wire_window* window)
{
	DXGI_SWAP_CHAIN_DESC scd;
	ID3D11Texture2D* pBackBuffer;
	HRESULT result;
	wire_window_internal* wnd;

	wnd = (wire_window_internal*) window;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Height = window->height;
	scd.BufferDesc.Width = window->width;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = wnd->assigned_window;
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
		&wnd->dx_swapchain,
		&wnd->d3d_dev,
		0,
		&wnd->d3d_devcontext);
	if (FAILED(result) || !wnd->dx_swapchain)
		return;

	wnd->dx_swapchain->lpVtbl->GetBuffer(wnd->dx_swapchain, 0, &IID_ID3D11Texture2D, (LPVOID*)&pBackBuffer);
	wnd->d3d_dev->lpVtbl->CreateRenderTargetView(wnd->d3d_dev, (ID3D11Resource*)pBackBuffer, 0, &wnd->d3d_backbuffer);
	pBackBuffer->lpVtbl->Release(pBackBuffer);
	wnd->d3d_devcontext->lpVtbl->OMSetRenderTargets(wnd->d3d_devcontext, 1, &wnd->d3d_backbuffer, 0);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 10;
	viewport.TopLeftY = 10;
	viewport.Width = (float)wnd->base.width;
	viewport.Height = (float)wnd->base.height;

	wnd->d3d_devcontext->lpVtbl->RSSetViewports(wnd->d3d_devcontext, 1, &viewport);
}

void render_frame(wire_window* window)
{
	D3DXCOLOR col;
	wire_window_internal* wnd;

	wnd = (wire_window_internal*) window;
	col.r = .4f;
	col.g = .4f;
	col.b = .1f;
	col.a = 1.0f;
	wnd->d3d_devcontext->lpVtbl->ClearRenderTargetView(wnd->d3d_devcontext, wnd->d3d_backbuffer, (const FLOAT*)&col);
	wnd->dx_swapchain->lpVtbl->Present(wnd->dx_swapchain, 0, 0);
}

void clear_d3d(wire_window* window)
{
	wire_window_internal* wnd;
	
	wnd = (wire_window_internal*) window;
	wnd->dx_swapchain->lpVtbl->SetFullscreenState(wnd->dx_swapchain, 0, 0);
	wnd->dx_swapchain->lpVtbl->Release(wnd->dx_swapchain);
	wnd->d3d_backbuffer->lpVtbl->Release(wnd->d3d_backbuffer);
	wnd->d3d_dev->lpVtbl->Release(wnd->d3d_dev);
	wnd->d3d_devcontext->lpVtbl->Release(wnd->d3d_devcontext);
}