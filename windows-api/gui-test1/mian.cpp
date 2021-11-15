#include <windows.h>

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK PanelProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void CenterWindow(HWND hwnd);
void RegisterBlueRectClass(void);
void RegisterRedRectClass(void);

constexpr int ID_HOT_KEY = 1;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd)
{
	MSG msg;
	HWND hwnd;

	//step 1, ÊµÀý»¯cls
	WNDCLASSW wcls = { 0 };
	wcls.hInstance = hInstance;
	wcls.lpszClassName = L"Windows";
	wcls.style = CS_HREDRAW | CS_VREDRAW;
	wcls.lpszMenuName = NULL;
	wcls.lpfnWndProc = WinProc;
	wcls.hbrBackground = GetSysColorBrush(COLOR_3DFACE);

	wcls.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcls.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	//step 2
	RegisterClassW(&wcls);
	hwnd = ::CreateWindowW(
		wcls.lpszClassName,
		L"Gui-Window",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		100, 100, 320, 220, 
		NULL, 
		NULL, 
		hInstance, 
		NULL);

	::ShowWindow(hwnd, nShowCmd);
	::UpdateWindow(hwnd);

	while (GetMessageW(&msg, hwnd, NULL, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessageW(&msg);
	}

	return (int) msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		RegisterBlueRectClass();
		CreateWindowW(L"BlueRect", NULL, WS_CHILD | WS_VISIBLE,
			20, 20, 80, 80,
			hwnd, (HMENU)1, NULL, NULL);

		RegisterRedRectClass();
		CreateWindowW(L"RedRect", NULL, WS_VISIBLE | WS_CHILD,
			120, 20, 80, 80,
			hwnd, (HMENU)2, NULL, NULL);
		RegisterHotKey(hwnd, ID_HOT_KEY, MOD_CONTROL, 0X43);
		break;

	case WM_DESTROY:
		UnregisterHotKey(hwnd, ID_HOT_KEY);
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			if (MessageBoxW(hwnd, L"Are you sure to quit?", L"Warning", MB_OKCANCEL) == IDOK)
			{
				::SendMessage(hwnd, WM_CLOSE, 0, 0);
			}
		}
		break;

	case WM_HOTKEY:
		if (wParam == ID_HOT_KEY)
		{
			CenterWindow(hwnd);
		}
		break;

	default:
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void CenterWindow(HWND hwnd)
{
	RECT rc = { 0 };
	::GetWindowRect(hwnd, &rc);

	int win_w = rc.right - rc.left;
	int win_h = rc.bottom - rc.top;

	int screen_w = ::GetSystemMetrics(SM_CXSCREEN);
	int screen_h = ::GetSystemMetrics(SM_CYSCREEN);
	::SetWindowPos(hwnd, HWND_TOP, (screen_w - win_w) / 2, (screen_h - win_h) / 2, 0, 0, SWP_NOSIZE);
}

LRESULT CALLBACK PanelProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_LBUTTONUP:
		MessageBeep(MB_OK);
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void RegisterBlueRectClass(void)
{
	HBRUSH hbrush = CreateSolidBrush(RGB(0, 0, 255));

	WNDCLASSW wcls = { 0 };
	wcls.lpszClassName = L"BlueRect";
	wcls.hbrBackground = hbrush;
	wcls.lpfnWndProc = PanelProc;
	wcls.hCursor = LoadCursor(NULL, IDC_ARROW);
	RegisterClassW(&wcls);
}

void RegisterRedRectClass(void)
{
	HBRUSH hbrush = CreateSolidBrush(RGB(255, 0, 0));

	WNDCLASSW wcls = { 0 };
	wcls.lpszClassName = L"RedRect";
	wcls.hbrBackground = hbrush;
	wcls.lpfnWndProc = PanelProc;
	wcls.hCursor = LoadCursor(NULL, IDC_ARROW);
	RegisterClassW(&wcls);
}