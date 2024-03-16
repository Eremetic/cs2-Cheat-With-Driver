#pragma once
#include "framework.h"
#include <dwmapi.h>

LRESULT CALLBACK ProcCallback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

class WindowCreation
{
public:

	RECT cs2Rect = { 0 };
	HWND cs2hWnd = NULL;
	boolean gameCheck = FALSE;

	BOOL WaitForGame();
	BOOL SingleInjection();
	
	///////////////////////////////////

	WNDCLASSEXW overlayClass = { 0 };
	HWND overlayhWnd = NULL;
	MARGINS margin = { 0 };
	UINT refreshRate = 0;
	
	BOOL InitOverlay();
	BOOL ResetWindZ();
	UINT Fetch_Framerate();

};

inline WindowCreation WC;

	


