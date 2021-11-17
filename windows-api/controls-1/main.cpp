#include <windows.h>
#include <direct.h>
#include <wchar.h>
#include <commctrl.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void LoadMyImage();
HBITMAP hBitMap;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	MSG msg;
	HWND hwnd;

	WNDCLASSW wc = { 0 };
	wc.lpszClassName = L"Window";
	wc.hInstance = hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpfnWndProc = WndProc;

	int win_w = 320;
	int win_h = 250;

	int screen_w = GetSystemMetrics(SM_CXSCREEN);
	int screen_h = GetSystemMetrics(SM_CYSCREEN);

	RegisterClassW(&wc);
	hwnd = CreateWindowW(wc.lpszClassName, L"Dylan", 
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		(screen_w - win_w)/2, (screen_h - win_h)/2, win_w, win_h, NULL, NULL, hInstance, 0);

	while (GetMessageW(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	const wchar_t* txt = L"My name is dylan";
	HWND hImage;
	switch (msg)
	{
	case WM_CREATE:
		CreateWindowW(WC_STATIC, txt, WS_CHILD | WS_VISIBLE, 10, 10, 200, 25, hwnd, (HMENU)1, NULL, NULL);
		LoadMyImage();
		hImage = CreateWindowW(WC_STATIC, L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, 20, 40, 300, 200, hwnd, (HMENU)2, NULL, NULL);
		SendMessage(hImage, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitMap);

		CreateWindowW(WC_BUTTON, L"beep", WS_VISIBLE | WS_CHILD, 200,150, 100, 35, hwnd, (HMENU)3, NULL, NULL);
		CreateWindowW(WC_BUTTON, L"message", WS_VISIBLE | WS_CHILD, 200, 200, 100, 35, hwnd, (HMENU)4, NULL, NULL);

		CreateWindowW(WC_BUTTON, L"check box", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 200, 300, 100, 35, hwnd, (HMENU)5, NULL, NULL);
		CheckDlgButton(hwnd, 5, BST_CHECKED);

		break;

	case WM_DESTROY:
		DeleteObject(hBitMap);
		PostQuitMessage(0);
		break;

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == 3)
		{
			MessageBeep(MB_OK);
		}
		else if (LOWORD(wParam) == 4)
		{
			MessageBoxW(hwnd, L"click msg button", L"MSG", MB_OKCANCEL);
		}
		else if (LOWORD(wParam) == 5)
		{
			bool checked = IsDlgButtonChecked(hwnd, 5);
			UINT chkState = checked ? BST_UNCHECKED : BST_CHECKED;
			
			CheckDlgButton(hwnd, 5, chkState);
		}
	}
		break;

	default:
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void LoadMyImage()
{
	wchar_t appDir[MAX_PATH];
	DWORD ret;
	ret = GetCurrentDirectoryW(MAX_PATH, appDir);
#if 0
	char exePath[1024] = { 0 };
	_getcwd(exePath, sizeof(exePath));
#endif
	wcscat_s(appDir, L"\\hw.bmp");
	hBitMap = (HBITMAP)LoadImageW(NULL, appDir, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	wprintf(L"current path:%ls", appDir);
}