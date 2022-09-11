#include "d3dsetup.h"

#include "wire_window_internal.h"
#include "renderobjects.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

#include <d3d10.h>
#include <d3dx10.h>
#include <d3d11.h>
#include <d3dx11.h>

int initialize_device(wire_window_internal* wnd)
{
	DXGI_SWAP_CHAIN_DESC scd;
	ID3D11Texture2D* pBackBuffer;
	HRESULT result;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Height = wnd->height;
	scd.BufferDesc.Width = wnd->width;
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
		&wnd->swapchain,
		&wnd->dev,
		0,
		&wnd->devcontext);
	if (FAILED(result) || !wnd->swapchain)
		return 1;

	wnd->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	wnd->dev->CreateRenderTargetView(pBackBuffer, 0, &wnd->backbuffer);
	pBackBuffer->Release();
	wnd->devcontext->OMSetRenderTargets(1, &wnd->backbuffer, 0);
	return 0;
}

void initialize_viewport(const wire_window_internal* wnd)
{
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 10;
	viewport.TopLeftY = 10;
	viewport.Width = (float)wnd->width;
	viewport.Height = (float)wnd->height;

	wnd->devcontext->RSSetViewports(1, &viewport);
}

void initialize_shader_pipeline(wire_window* window)
{
	wire_window_internal* wnd;
	
	wnd = (wire_window_internal*) window;
	ID3D10Blob *vertex_shader, *pixel_shader;
	D3DX11CompileFromFileA("shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &vertex_shader, 0, 0);
	D3DX11CompileFromFileA("shaders.shader", 0, 0, "PShader", "vs_4_0", 0, 0, 0, &pixel_shader, 0, 0);
	
	wnd->dev->CreateVertexShader(vertex_shader->GetBufferPointer(), 
								 vertex_shader->GetBufferSize(),
								 0, &wnd->vertex_shader);
	wnd->dev->CreatePixelShader(pixel_shader->GetBufferPointer(), 
								pixel_shader->GetBufferSize(),
								0, &wnd->pixel_shader);

	wnd->devcontext->VSSetShader(wnd->vertex_shader, 0, 0);
	wnd->devcontext->PSSetShader(wnd->pixel_shader, 0, 0);
}

void initialize_input_layout(wire_window_internal wnd)
{
	D3D11_INPUT_ELEMENT_DESC il[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
}

void initialize_d3d(wire_window* window)
{
	wire_window_internal* wnd;
	D3D11_BUFFER_DESC bd;
	
	wnd = (wire_window_internal*) window;
	if (initialize_device(wnd))
		return;
	initialize_viewport(wnd);
	initialize_shader_pipeline(window);

	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(vertex) * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	wnd->dev->CreateBuffer(&bd, NULL, &wnd->vertex_buffer);
	
	vertex vs[3] = {
		{10, 10, 10, {1, 0, 0, 1}},
		{20, 20, 20, {0, 1, 0, 1}},
		{60, 60, 60, {0, 0, 1, 1}},
	};

	D3D11_MAPPED_SUBRESOURCE mapped_res;
	wnd->devcontext->Map(wnd->vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
	memcpy(mapped_res.pData, vs, sizeof(vertex) * 3);
	wnd->devcontext->Unmap(wnd->vertex_buffer, 0);
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
	wnd->devcontext->ClearRenderTargetView(wnd->backbuffer, (const FLOAT*)&col);
	wnd->swapchain->Present(0, 0);
}

void clear_d3d(wire_window* window)
{
	wire_window_internal* wnd;
	
	wnd = (wire_window_internal*) window;
	wnd->swapchain->SetFullscreenState(0, 0);

	wnd->pixel_shader->Release();
	wnd->vertex_shader->Release();
	wnd->swapchain->Release();
	wnd->backbuffer->Release();
	wnd->dev->Release();
	wnd->devcontext->Release();
}