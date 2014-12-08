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
	
	return true;
}

bool GraphicsClass::Frame()
{
	return true;
}

void GraphicsClass::Shutdown()
{
	
}
bool GraphicsClass::Render()
{
	return true;
}