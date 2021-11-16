#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK PanelProc(HWND, UINT, WPARAM, LPARAM);

void RegisterPanel(void);

COLORREF gcolor = RGB(255, 255, 0);

COLORREF ShowColorDialog(HWND hwnd);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	MSG msg;
	HWND hwnd;

	WNDCLASS wc = { 0 };
	wc.lpszClassName = L"Window";
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.lpfnWndProc = WndProc;

	RegisterClass(&wc);

	hwnd = CreateWindow(wc.lpszClassName, L"Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 320, 250, NULL, NULL, hInstance, 0);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

void RegisterPanel()
{
	WNDCLASS wc = { 0 };
	wc.lpszClassName = L"Panel";
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpfnWndProc = PanelProc;
	RegisterClass(&wc);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndPanel;

	switch (msg)
	{
	case WM_CREATE:
		//创建一个按钮
		CreateWindow(L"button", L"Color", WS_CHILD | WS_VISIBLE, 10, 10, 50, 25, hwnd, (HMENU)1, NULL, 0);
		RegisterPanel();

		//创建一个矩形区域
		hwndPanel = CreateWindow(L"Panel", NULL, WS_CHILD | WS_VISIBLE, 100, 10, 100, 100, hwnd, (HMENU)2, NULL, NULL);
		break;

	case WM_COMMAND:
		gcolor = ShowColorDialog(hwnd);
		InvalidateRect(hwndPanel, NULL, TRUE);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK PanelProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;

	switch (msg)
	{
	case WM_PAINT:
	{
		GetClientRect(hwnd, &rect);
		hdc = BeginPaint(hwnd, &ps);
		SetBkColor(hdc, gcolor);
		ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rect, L"", 0, NULL);
		EndPaint(hwnd, &ps);
	}
	break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

COLORREF ShowColorDialog(HWND hwnd)
{
	CHOOSECOLOR cc = {0};
	static COLORREF lastColorBuf[16];

	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = hwnd;
	cc.lpCustColors = (LPDWORD)lastColorBuf;
	cc.rgbResult = RGB(0, 255, 0);
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	ChooseColor(&cc);

	return cc.rgbResult;
}

