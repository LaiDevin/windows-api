#include <windows.h>

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HINSTANCE gHinstance;

void CreateDialogClass(HWND);
void RegisterDialogClass(HWND);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	MSG msg;
	HWND hwnd;

	WNDCLASS wcls = { 0 };
	wcls.lpszClassName = L"Window";
	wcls.lpfnWndProc = WinProc;
	wcls.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wcls.hInstance = hInstance;
	gHinstance = hInstance;

	RegisterClass(&wcls);
	hwnd = CreateWindow(wcls.lpszClassName, L"Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 250, 320, NULL, NULL, hInstance, 0);
	if (!hwnd)
	{
		MessageBox(NULL, L"hwnd is invalid", L"Error", MB_OK);
		return -1;
	}

	//这里第2个参数要为NULL
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		RegisterDialogClass(hwnd);
		CreateWindowW(L"button", L"Show dialog",
			WS_VISIBLE | WS_CHILD,
			20, 50, 95, 25, hwnd, (HMENU)1, NULL, NULL);
		break;

	case WM_COMMAND:
		CreateDialogClass(hwnd);
		break;

	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {

	case WM_CREATE:
		CreateWindowW(L"button", L"Ok",
			WS_VISIBLE | WS_CHILD,
			50, 50, 80, 25, hwnd, (HMENU)1, NULL, NULL);
		break;

	case WM_COMMAND:
		DestroyWindow(hwnd);
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	}

	return (DefWindowProcW(hwnd, msg, wParam, lParam));
}

void CreateDialogClass(HWND hwnd)
{
	CreateWindowExW(WS_EX_DLGMODALFRAME | WS_EX_TOPMOST, L"DialogClass", L"Dialog Box",
		WS_VISIBLE | WS_SYSMENU | WS_CAPTION, 100, 100, 200, 150,
		NULL, NULL, gHinstance, NULL);
}

void RegisterDialogClass(HWND hwnd)
{
	WNDCLASSEXW wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.lpfnWndProc = (WNDPROC)DialogProc;
	wc.hInstance = gHinstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpszClassName = L"DialogClass";
	RegisterClassExW(&wc);
}