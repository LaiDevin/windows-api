#include <windows.h>
#include <wchar.h>
#include <strsafe.h>
#include <commctrl.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CenterWindow(HWND);

void RegisterBlueRectClass();
void RegisterRedRectClass();

LRESULT CALLBACK PanelProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND hwndStat1, hwndStat2;

HWND hwndMenu2;

void CreateLabels(HWND hwnd);

constexpr int ID_HOTKEY = 1;
constexpr int IDM_FILE_NEW = 1;
constexpr int IDM_FILE_OPEN = 2;
constexpr int IDM_FILE_QUIT = 3;

//开关菜单
constexpr int IDM_SIWITCH_ON = 30;

//日期菜单
constexpr int IDM_WEEK_MON = 41;
constexpr int IDM_WEEK_TUE = 42;
constexpr int IDM_WEEK_WEN = 43;

constexpr int IDM_FILE_IMPORT = 50;

HMENU hMenuOn;
HWND hgsb;

HMENU hWeekMenu;

//菜单
void AddMenus(HWND hwnd);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ PWSTR pCmdLine, _In_ int nCmdShow) 
{

	MSG  msg;
	HWND hwnd;
	WNDCLASSW wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpszClassName = L"Window";
	wc.hInstance = hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpszMenuName = NULL;
	wc.lpfnWndProc = WndProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassW(&wc);
	hwnd = CreateWindowW(wc.lpszClassName, L"Window",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		100, 100, 350, 250, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {

	RECT rc = { 0 };
	wchar_t buf[10] = { 0 };

	switch (msg) {

	case WM_DESTROY:
		UnregisterHotKey(hwnd, ID_HOTKEY);
		PostQuitMessage(0);
		return 0;

	case WM_CREATE:
		RegisterBlueRectClass();

		//要和class名字一致
		CreateWindowW(L"BlueClass",
			NULL,
			WS_CHILD | WS_VISIBLE,
			20, 20, 80, 80,
			hwnd, (HMENU)1, NULL, NULL);

		RegisterRedRectClass();

		//要和class名字一致
		CreateWindowW(L"RedClass", NULL,
			WS_CHILD | WS_VISIBLE,
			120, 20, 80, 80,
			hwnd, (HMENU)2, NULL, NULL);

		AddMenus(hwnd);

		hgsb = CreateWindowExW(0, STATUSCLASSNAMEW, NULL,
			WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hwnd,
			(HMENU)1, GetModuleHandle(NULL), NULL);

		CreateLabels(hwnd);
		
		//0x43 C KEY
		RegisterHotKey(hwnd, ID_HOTKEY, MOD_CONTROL, 0X43);
		break;

	case WM_MOVE:
		GetWindowRect(hwnd, &rc);
		StringCbPrintfW(buf, sizeof(buf), L"%ld", rc.left);
		SetWindowTextW(hwndStat1, buf);

		StringCbPrintfW(buf, sizeof(buf), L"%ld", rc.top);
		SetWindowTextW(hwndStat2, buf);

		break;

		//处理快捷键
	case WM_HOTKEY:
		if (wParam == ID_HOTKEY)
		{
			CenterWindow(hwnd);
		}
		break;

		//处理键盘按下
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			int ret = MessageBoxW(hwnd, L"Are you sure to quit?", L"Information", MB_OKCANCEL);
			if (ret == IDOK)
			{
				::SendMessage(hwnd, WM_CLOSE, 0, 0);
			}
		}
		break;

		//处理命令消息
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_FILE_NEW:
		case IDM_FILE_OPEN:
			MessageBeep(MB_ICONINFORMATION);
			break;

		case IDM_FILE_QUIT:
			::SendMessage(hwnd, WM_CLOSE, 0, NULL);
			break;

		case IDM_FILE_IMPORT:
			MessageBoxW(NULL, L"Test Import", L"success", MB_OK);
			break;

		case IDM_SIWITCH_ON: {
			int state = GetMenuState(hMenuOn, IDM_SIWITCH_ON, MF_BYCOMMAND);
			if (state == MF_CHECKED)
			{
				ShowWindow(hgsb, SW_HIDE);
				CheckMenuItem(hMenuOn, IDM_SIWITCH_ON, MF_UNCHECKED);
			}
			else
			{
				ShowWindow(hgsb, SW_SHOWNA);
				CheckMenuItem(hMenuOn, IDM_SIWITCH_ON, MF_CHECKED);
			}
		}
			break;

		case IDM_WEEK_MON:
			CheckMenuRadioItem(hWeekMenu, IDM_WEEK_MON, IDM_WEEK_WEN, IDM_WEEK_MON, MF_BYCOMMAND);
			MessageBeep(MB_ICONERROR);
			break;

		case IDM_WEEK_TUE:
			CheckMenuRadioItem(hWeekMenu, IDM_WEEK_MON, IDM_WEEK_WEN, IDM_WEEK_TUE, MF_BYCOMMAND);
			MessageBeep(MB_ICONINFORMATION);
			break;

		case IDM_WEEK_WEN:
			CheckMenuRadioItem(hWeekMenu, IDM_WEEK_MON, IDM_WEEK_WEN, IDM_WEEK_WEN, MF_BYCOMMAND);
			MessageBeep(MB_ICONWARNING);
			break;
		}
		break;

	case WM_RBUTTONUP: {
#if 0
		MessageBoxW(hwnd, L"SHIT", L"msg", MB_OK);
#endif
		HMENU hPopupMenu = CreatePopupMenu();
		POINT pt = { LOWORD(lParam), HIWORD(lParam) };
		ClientToScreen(hwnd, &pt);

		AppendMenuW(hPopupMenu, MF_STRING, IDM_FILE_NEW, L"&New");
		AppendMenuW(hPopupMenu, MF_STRING, IDM_FILE_OPEN, L"&Open");
		AppendMenuW(hPopupMenu, MF_SEPARATOR, 0, NULL);
		AppendMenuW(hPopupMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit");

		TrackPopupMenu(hPopupMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
		DestroyMenu(hPopupMenu);
	}
		break;

	case WM_SIZE:
		::SendMessageW(hgsb, WM_SIZE, wParam, lParam);
		break;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void CenterWindow(HWND hwnd)
{
	RECT rc = { 0 };
	GetWindowRect(hwnd, &rc);
	
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	int screen_x = GetSystemMetrics(SM_CXSCREEN);
	int screen_y = GetSystemMetrics(SM_CYSCREEN);

	SetWindowPos(hwnd, HWND_TOP, (screen_x - width) / 2, (screen_y - height) / 2, 0, 0, SWP_NOSIZE);
}

void RegisterBlueRectClass()
{
	HBRUSH hbrush = CreateSolidBrush(RGB(0, 0, 255));

	WNDCLASSW rwc = { 0 };
	rwc.lpszClassName = L"BlueClass";
	rwc.hbrBackground = hbrush;
	rwc.lpfnWndProc = PanelProc;
	rwc.hCursor = LoadCursor(0, IDC_ARROW);
	RegisterClassW(&rwc);
}

void RegisterRedRectClass()
{
	HBRUSH hbrush = CreateSolidBrush(RGB(255, 0, 0));
	WNDCLASSW rwc = { 0 };
	rwc.lpszClassName = L"RedClass";
	rwc.hbrBackground = hbrush;
	rwc.lpfnWndProc = PanelProc;
	rwc.hCursor = LoadCursor(0, IDC_ARROW);
	RegisterClassW(&rwc);
}

LRESULT CALLBACK PanelProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_LBUTTONUP:
		MessageBeep(MB_OK);
		break;
	}
	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void CreateLabels(HWND hwnd)
{
	CreateWindowW(L"static", L"X:",
		WS_CHILD | WS_VISIBLE,
		10, 100, 25, 125,
		hwnd, (HMENU)1, NULL, NULL);

	hwndStat1 = CreateWindowW(L"static", L"150",
		WS_CHILD | WS_VISIBLE,
		40, 100, 55, 125,
		hwnd, (HMENU)2, NULL, NULL);

	CreateWindowW(L"static", L"Y:",
		WS_CHILD | WS_VISIBLE,
		10, 130, 25, 125,
		hwnd, (HMENU)3, NULL, NULL);

	hwndStat2 = CreateWindowW(L"static", L"150",
		WS_CHILD | WS_VISIBLE,
		40, 130, 55, 125,
		hwnd, (HMENU)4, NULL, NULL);
}

void AddMenus(HWND hwnd)
{
	HMENU hMenuBar, hMenu;

	hMenuBar = CreateMenu();
	hMenu = CreateMenu();

	hMenuOn = CreateMenu();
	hWeekMenu = CreateMenu();

	HMENU hImportMenu = CreatePopupMenu();

	AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
	AppendMenuW(hMenu, MF_STRING, IDM_FILE_OPEN, L"&Open");

	AppendMenuW(hMenu, MF_STRING | MF_POPUP, (UINT_PTR) hImportMenu, L"Import");
	AppendMenuW(hImportMenu, MF_STRING, IDM_FILE_IMPORT, L"Import &local file");

	AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
	AppendMenuW(hMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit");

	AppendMenuW(hMenuOn, MF_STRING, IDM_SIWITCH_ON, L"&ON");
	CheckMenuItem(hMenuOn, IDM_SIWITCH_ON, MF_CHECKED);

	AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hMenu, L"&File");
	AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hMenuOn, L"&Help");

	AppendMenuW(hWeekMenu, MF_STRING, IDM_WEEK_MON, L"星期一");
	AppendMenuW(hWeekMenu, MF_STRING, IDM_WEEK_TUE, L"星期二");
	AppendMenuW(hWeekMenu, MF_STRING, IDM_WEEK_WEN, L"星期三");

	AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hWeekMenu, L"日期");
	CheckMenuRadioItem(hWeekMenu, IDM_WEEK_MON, IDM_WEEK_WEN, IDM_WEEK_TUE, MF_BYCOMMAND);

	::SetMenu(hwnd, hMenuBar);
}
