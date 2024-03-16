#include "WindowCreate.hpp"
#include <random>
#include "utils.hpp"
#include "Driver.h"
#include <sstream>

namespace RandNum
{
	char CHARSET[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

	static std::default_random_engine random{ static_cast<unsigned>(time(0)) };
	static std::mt19937 random_generator(random());

	static int Rand()
	{
#pragma warning( push )
#pragma warning( disable : 4244)
		srand(time(NULL));
		int i = (rand() % (20 - 10 + 1)) + 10;
#pragma warning( pop ) 
		return i;
	}

	static std::string generate(size_t length)
	{

		std::string str = CHARSET;

		while (length > str.length()) str += str;

		std::shuffle(str.begin(), str.end(), random_generator);

		return str.substr(0, length);
	}

}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	static int tLength = GetWindowTextLengthW(hWnd);
	WCHAR* windowTitle = new WCHAR[tLength + 1];
	
	GetWindowTextW(hWnd, windowTitle, tLength);

	if (wcscmp(windowTitle, L"Counter-Strike 2") == 0)
	{
		if(WC.cs2hWnd == NULL) WC.cs2hWnd = hWnd;
	
		return FALSE;
	}

	return TRUE;
}


BOOL WindowCreation::SingleInjection()
{
	EnumWindows(EnumWindowsProc, 0);

	if(this->cs2hWnd != NULL) return TRUE;
	
	return FALSE;
}


BOOL WindowCreation::WaitForGame()
{
	Timer time{};
	int sleepCount = 0;
	time.start();
	while (!this->gameCheck)
	{
		
		this->cs2hWnd = FindWindowW(NULL, L"Counter-Strike 2");
		if (this->cs2hWnd)
		{
			if (!GetWindowRect(this->cs2hWnd, &this->cs2Rect))
			{
				Logger.Logger("Failed To Obtain Counter Strike 2 Window Size\n");
				return FALSE;
			}
			this->gameCheck = TRUE;
			return TRUE;
		}

			
		if (time.elapsedSeconds() > 90)
		{
			Logger.Logger("Failed To Obtain Counter Strike 2 Window Handle\n");
			return FALSE;
		}
	}

	return TRUE;
}


BOOL WindowCreation::InitOverlay()
{
	std::string random = RandNum::generate(RandNum::Rand());
	std::wstring conv(random.begin(), random.end());
	LPCWSTR className = conv.c_str();

	std::memset(&this->overlayClass, 0, sizeof(WNDCLASSEXW));

	this->overlayClass.cbSize = sizeof(WNDCLASSEXW);
	this->overlayClass.hInstance = NULL;
	this->overlayClass.lpszClassName = className;
	this->overlayClass.cbClsExtra = NULL;
	this->overlayClass.cbWndExtra = NULL;
	this->overlayClass.hbrBackground = (HBRUSH)RGB(0, 0, 0);
	this->overlayClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
	this->overlayClass.hIcon = LoadIconW(0, IDI_APPLICATION);
	this->overlayClass.hIconSm = LoadIconW(0, IDI_APPLICATION);
	this->overlayClass.lpszMenuName = className;
	this->overlayClass.lpfnWndProc = ProcCallback;
		
	RegisterClassExW(&this->overlayClass);


	LONG width = this->cs2Rect.right - this->cs2Rect.left,
		height = this->cs2Rect.bottom - this->cs2Rect.top;


	this->overlayhWnd = CreateWindowExW(NULL,
		className, className, WS_POPUP | WS_VISIBLE,
		this->cs2Rect.left, this->cs2Rect.top,
		width, height, NULL, NULL, NULL, NULL);
	
	this->margin = { 0, 0, width, height };

	HRESULT result = DwmExtendFrameIntoClientArea(this->overlayhWnd, &this->margin);
	if (result == S_OK)
	{
		SetWindowLongPtr(this->overlayhWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT);

		driver.Set_Window_Protection(this->overlayhWnd, 0x00000011);

		return TRUE;
	}
	
	this->refreshRate = this->Fetch_Framerate();

	Logger.Logger("Failed To Initailize Overlay\n");
	return FALSE;
}



BOOL WindowCreation::ResetWindZ()
{
	if (IsWindowVisible(this->cs2hWnd))
	{
		if (SetWindowPos(this->cs2hWnd, this->overlayhWnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE))
		{
			if (!GetWindowRect(this->cs2hWnd, &this->cs2Rect))
			{
				Logger.Logger("Failed to Update Window Rect\n");
				return FALSE;
			}
			
			return TRUE;
		}
	}
	
	SetWindowPos(this->overlayhWnd, this->cs2hWnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	return TRUE;
}


LRESULT ProcCallback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProcW(hWnd, Msg, wParam, lParam);
}

UINT WindowCreation::Fetch_Framerate()
{
	DEVMODEW devMode = { 0 };

	if (!EnumDisplaySettingsExW(nullptr, ENUM_CURRENT_SETTINGS, &devMode, EDS_RAWMODE))
	{
		LONG error = GetLastError();
		std::ostringstream oss;
		oss << "Failed to Enum DIsplay Setting With ERROR : " << error << "\n";
		Logger.Logger(oss.str());
		return NULL;
	}

	return devMode.dmDisplayFrequency;
}


