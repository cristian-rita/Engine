#include "systemclass.h"

SystemClass::SystemClass()
{
	m_Input = 0;
	m_Graphics = 0;
}

SystemClass::SystemClass(const SystemClass& other)
{

}

SystemClass::~SystemClass()
{

}

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	screenWidth = 600;
	screenHeight = 600;

	/*Initialize windows API*/

	InitializeWindows(screenWidth, screenWidth);
	/*Create input object*/
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	/*Initialize input object*/
	m_Input->Initialize();

	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}
	/*Initialize graphics object*/
	result = m_Graphics->Initialize(screenWidth, screenHeight,m_hwnd);
	if (!result)
	{
		return false;
	}
	return true;
}	

void SystemClass::Shutdown()
{
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	ShutdownWindows();
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;
	ZeroMemory(&msg, sizeof(MSG));
	//Initialize the message structure
	done = false;
	while (!done)
	{
		//Handle the windows message
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//If windows signals to end the application
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}
}

bool SystemClass::Frame()
{
	bool result;
	
	/*Check if the user pressed ESC and wants to exit*/
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}
	/*Do the frame processing*/
	result = m_Graphics->RenderFrame();
	if (!result)
	{
		return false;
	}
	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		/*Check if a key has been pressed on keyboard*/
	case WM_KEYDOWN:
	{
		/*If a key is pressed send it to the input object so it can record the state*/
		m_Input->KeyDown((unsigned int)wparam);
		return 0;
	}
	case WM_KEYUP:
	{
		/*If a key is released send it to the input object so it can unset the state*/
		m_Input->KeyUp((unsigned int)wparam);
		return 0;
	}
	default:
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}

void SystemClass::InitializeWindows(int screenWidth,int screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int	posX = 0, posY = 0;

	/*Get an external pointer to this object*/
	ApplicationHandle = this;

	/*Get the instance of this application*/
	m_hInstance = GetModuleHandle(NULL);

	/*Give the application a name*/
	m_applicationName = L"Engine";

	/*Setup the windows class with default settings*/
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszClassName = m_applicationName;
	wc.lpszMenuName = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc;
	/*Register window class*/

	RegisterClassEx(&wc);

	

	if (FULL_SCREEN)
	{
		/*Determine resolution */
		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);

		/*If full screen set the screen to the maximum size*/
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = screenWidth;
		dmScreenSettings.dmPelsHeight = screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSHEIGHT | DM_PELSWIDTH;

		/*Change display settings to full screen*/
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		/*Set the position of the windows to the left corner*/
		posX = 0;
		posY = 0;
	}
	else 
	{
		//screenHeight = 750;
		//screenWidth = 800;

		/*Place window in the middle of the screen*/
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	}

	/*Create the window with the screen settings and get the handle to it*/



	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_OVERLAPPEDWINDOW, posX, posY,
		screenWidth, screenHeight, NULL, NULL, m_hInstance, NULL);

	/*Bring the window up to the screen*/
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	/*Hide mouse cursor*/
	//ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	ShowCursor(true);

	/*Fix the display settings if leaving fullscreen*/
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}
	/*Remove the window*/
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	/*Remove the application instance*/
	UnregisterClass(m_applicationName, m_hInstance);
	m_hInstance = NULL;

	/*Release the pointer to this class*/

	ApplicationHandle = NULL;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}
