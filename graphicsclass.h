#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_
#include<windows.h>
#include <d3d11.h>
#include <windowsx.h>

#pragma comment(lib,"d3d11.lib")


const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
	IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
	ID3D11Device *dev;                     // the pointer to our Direct3D device interface
	ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();
};
#endif // !_GRAPHICSCLASS_H_
