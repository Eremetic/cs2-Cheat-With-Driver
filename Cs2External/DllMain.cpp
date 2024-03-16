#include "Overlay.hpp"
#include "Driver.h"
#include "utils.hpp"


///threadlibrarydisable
HMODULE g_hModule = nullptr;

#ifdef _WINDLL

static DWORD WINAPI MainThread(LPVOID lpReserved)
{
	Logger.myDocs();
	if (!driver.Init())
	{
		Logger.Logger("Main Thread Closing Due To Failed Driver Initilization\n");
		return FALSE;
	}

	if (!WC.WaitForGame())
	{
		Logger.Logger("Main Thread Closed Due Not Finding Game Window\n");
		return FALSE;
	}

	std::this_thread::sleep_for(3000ms); ///letting game load virtual memory

	if (!WC.InitOverlay())
	{
		Logger.Logger("Main Thread Closed Due to Overlay Failure\n");
		return FALSE;
	}

	Overlay overlay{};

	while (overlay.msg_loop() && !overlay.ready)
	{
		if (!overlay.CheatsCheck) break;
	}

	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		g_hModule = hModule;
		CreateThread(nullptr, 0, MainThread, g_hModule, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}

#else

int WinMain(
HINSTANCE   hInstance,
HINSTANCE   hPrevInstance,
LPSTR       lpCmdLine,
int         nCmdShow
)
{
		
	Logger.myDocs();
	if (!driver.Init())
	{
		Logger.Logger("Main Thread Closing Due To Failed Driver Initilization\n");
		return FALSE;
	}
	
	if (!WC.WaitForGame())
	{
		Logger.Logger("Main Thread Closed Due Not Finding Game Window\n");
		return FALSE;
	}

	std::this_thread::sleep_for(3000ms); ///letting game load virtual memory

	if (!WC.InitOverlay())
	{
		Logger.Logger("Main Thread Closed Due to Overlay Failure\n");
		return FALSE;
	}

	Overlay overlay{};

	while (overlay.msg_loop() && !overlay.ready) 
	{
		if (!overlay.CheatsCheck) break;
	}

	return 1;
}

#endif
