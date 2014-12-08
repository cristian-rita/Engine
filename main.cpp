#include "systemclass.h"

int WINAPI WinMain(HINSTANCE Hinstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdShow)
{
	SystemClass *System;
	bool result;

	/*Create system object*/
	System = new SystemClass;
	if (!System)
	{
		return 0;
	}
	result = System->Initialize();
	if (result)
	{
		System->Run();
	}
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}