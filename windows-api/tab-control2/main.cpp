#include <windows.h>
#include <wchar.h>
#include <CommCtrl.h>
#include <iostream>

using namespace std;

#pragma comment(lib, "comctl32.lib")

HINSTANCE ghinstance;
HWND hTab;
HWND hwndMain;

int ADD_TAB_BTN_ID = 40;
int START_TAB_POS = 100;


void ApppendMyTab(HWND myTab)
{
	if (myTab == INVALID_HANDLE_VALUE)
	{
		cout << "not valid" << endl;
		return;
	}

	int len = TabCtrl_GetItemCount(myTab);
	cout << "len: " << len << endl;

	TCITEM it = { 0 };
	it.mask = TCIF_TEXT;
	it.pszText = const_cast<char*>("fd");
	TabCtrl_InsertItem(myTab, len, &it);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		CreateWindow(WC_BUTTON, "ADD", WS_VISIBLE | WS_CHILD | WS_BORDER,
			10, 10, 80, 28, hwnd, (HMENU)ADD_TAB_BTN_ID, ghinstance, 0);

		INITCOMMONCONTROLSEX icce = { 0 };
		icce.dwSize = sizeof(INITCOMMONCONTROLSEX);
		icce.dwICC = ICC_TAB_CLASSES;
		InitCommonControlsEx(&icce);
		hTab = CreateWindow(WC_TABCONTROL, "", WS_CLIPSIBLINGS | WS_VISIBLE | WS_CHILD,
			10, 50, 400, 300, hwnd, (HMENU)10, ghinstance, 0);

		CHAR name[] = "dylan";
		TCITEM it;
		it.mask = TCIF_TEXT;
		it.pszText = name;
		TabCtrl_InsertItem(hTab, 0 ,&it);
	}
		break;

	case WM_COMMAND:
	{
		int id = LOWORD(wParam);
		if (id == ADD_TAB_BTN_ID)
		{
#if 0
			MessageBoxW(hwnd, L"info", L"Title", MB_OK);
#endif
			ApppendMyTab(hTab);
		}
	}
		break;

	case WM_NOTIFY:
	{
		int id = LOWORD(wParam);
		if (id == 10)
		{
			LPNMHDR lpnmhdr = (LPNMHDR)lParam;
			switch (lpnmhdr->code)
			{
			case TCN_SELCHANGE:
			{
				//MessageBox(hwnd, "ddd", "dd", MB_OK);
				HWND _hwnd = GetDlgItem(hwnd, lpnmhdr->idFrom);
				TabCtrl_GetCurSel(hTab);
			}
				break;
			default:
				break;
			}
		}
	}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	MSG msg;
	WNDCLASS wc = { 0 };
	wc.hInstance = hInstance;
	wc.lpszClassName = "Window";
	wc.lpfnWndProc = WndProc;
	wc.hbrBackground = HBRUSH(COLOR_3DFACE + 1);
	ghinstance = hInstance;

	RegisterClass(&wc);
	hwndMain = CreateWindow(wc.lpszClassName, "Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 360, NULL, NULL, hInstance, 0);

	while (GetMessageW(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}