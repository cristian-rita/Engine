#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{

}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{

}

GraphicsClass::~GraphicsClass()
{

}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	color[0] = 0.0f;
	color[1] = 0.2f;
	color[2] = 0.0f;
	color[3] = 0.0f; 
	/*Create a struct to hold the info about swap chain*/
	DXGI_SWAP_CHAIN_DESC scd;

	/*Clear the struct*/

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;										//one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;			//use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;			//how swap chain is to be used
	scd.OutputWindow = hwnd;									//the window to be used
	scd.SampleDesc.Count = 4;									//how many multi samples
	scd.Windowed = TRUE;										//windowed / full screen

	/*Create device, device context and swap chain */

	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&dev,
		NULL,
		&devcon);
	
	/*Get the address of the back buffer*/
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	/*Use the back buffer address to create the render target*/
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	/*Set the render target as back buffer*/
	devcon->OMSetRenderTargets(1, &backbuffer, NULL);

	/*Set the viewport*/
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Height = screenHeight;
	viewport.Width = screenWidth;

	devcon->RSSetViewports(1, &viewport);
	return true;
}

bool GraphicsClass::RenderFrame()
{
	/*Clear the back buffer to a color*/
	devcon->ClearRenderTargetView(backbuffer, color); 
	if(color[2] < 1.0f)
		color[2] += 0.001;
	swapchain->Present(0, 0);
	return true;
}

void GraphicsClass::Shutdown()
{
	swapchain->Release();
	dev->Release();
	devcon->Release();
	backbuffer->Release();
}
