/**
* @brief:
* 这个章节处理edit, Trackbar Tooltip
**/
#include <Windows.h>
#include <CommCtrl.h>
#include <wchar.h>
#include <strsafe.h>

#pragma comment(lib, "comctl32.lib")

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

constexpr int ID_MY_DEIT = 10;
constexpr int ID_MY_BUTTON = 11;

#define  ID_MY_UP_DOWN 20
#define  ID_MY_UP_EDIT 21
#define  ID_MY_UPDOWN_LABLE 22

#define UD_MAX_POS 30
#define UD_MIN_POS 0

void CreateMyTrackBar(HWND hwnd);
void UpdateMyTrackBar();
void CreateMyUpdown(HWND hwnd);
void UpdateMyUpdown();

HWND hUpdownLabel;
HWND hUpdown;
HWND hUpdownEdit;

HWND hTrackBar;
HWND hDataShowLbl;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	MSG msg;
	HWND hwnd;

	WNDCLASSW wc = { 0 };
	wc.lpszClassName = L"Window";
	wc.hInstance = hInstance;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpfnWndProc = WndProc;

	RegisterClassW(&wc);
	hwnd = CreateWindowW(wc.lpszClassName, L"Dylan", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		300, 300, 320, 250, NULL, NULL, hInstance, 0);

	while (GetMessageW(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND hMyBtn;
	static HWND hEdit;
	LPNMUPDOWN lpnmud;
	UINT code;

	switch (msg)
	{
	case WM_NCCREATE:
		break;

	case  WM_CREATE:
		hEdit = CreateWindowW(WC_EDIT, NULL, WS_VISIBLE | WS_CHILD | WS_BORDER,
			10, 10, 150, 30, hwnd, (HMENU)ID_MY_DEIT, NULL, NULL);
		hMyBtn = CreateWindowW(WC_BUTTON, L"click", WS_VISIBLE | WS_CHILD,
			10, 50, 60, 30, hwnd, (HMENU)ID_MY_BUTTON, NULL, NULL);
		CreateMyTrackBar(hwnd);
		CreateMyUpdown(hwnd);
		break;

	case WM_COMMAND:
	{
		if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == ID_MY_BUTTON)
		{
			TCHAR txt[128];
			GetWindowTextW(hEdit, txt, 128);
#if 0
			int len = GetWindowTextLengthW(hEdit) + 1;
			wchar_t* text = new wchar_t[len];
#endif

			GetWindowTextW(hEdit, txt, 128);
			SetWindowTextW(hwnd, txt);
		}
	}
		break;

	case WM_NOTIFY:
	{
		code = ((LPNMHDR)lParam)->code;
		if (code == UDN_DELTAPOS) {

			lpnmud = (NMUPDOWN*)lParam;

			int value = lpnmud->iPos + lpnmud->iDelta;

			if (value < UD_MIN_POS) {
				value = UD_MIN_POS;
			}

			if (value > UD_MAX_POS) {
				value = UD_MAX_POS;
			}

			const int asize = 4;
			wchar_t buf[asize];
			size_t cbDest = asize * sizeof(wchar_t);
			StringCbPrintfW(buf, cbDest, L"%d", value);

			SetWindowTextW(hUpdownLabel, buf);
		}
	}
		break;

	case WM_HSCROLL:
		UpdateMyTrackBar();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void CreateMyTrackBar(HWND hwnd)
{
	HWND hLeftLabel = CreateWindowW(WC_STATIC, L"0", WS_CHILD | WS_VISIBLE, 0, 0, 10, 30, hwnd, NULL, NULL, 0);
	HWND hRightLabel = CreateWindowW(WC_STATIC, L"100", WS_CHILD | WS_VISIBLE, 0, 0, 30, 30, hwnd, NULL, NULL, 0);

	hDataShowLbl = CreateWindowW(WC_STATIC, L"0", WS_CHILD | WS_VISIBLE, 300, 300, 30, 30, hwnd, NULL, NULL, 0);

	INITCOMMONCONTROLSEX exs = {0};
	exs.dwSize = sizeof(INITCOMMONCONTROLSEX);
	exs.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&exs);

	hTrackBar = CreateWindowW(TRACKBAR_CLASSW, L"track bar", WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
		20, 300, 170, 30, hwnd, (HMENU)3, NULL, NULL);
	SendMessage(hTrackBar, TBM_SETRANGE, TRUE, MAKELONG(0, 100));
	SendMessage(hTrackBar, TBM_SETPAGESIZE, 0, 10);
	SendMessage(hTrackBar, TBM_SETTICFREQ, 10, 0);
	SendMessage(hTrackBar, TBM_SETBUDDY, TRUE, (LPARAM)hLeftLabel);
	SendMessage(hTrackBar, TBM_SETBUDDY, FALSE, (LPARAM)hRightLabel);
	SendMessage(hTrackBar, TBM_SETPOS, FALSE, 0);
}

void UpdateMyTrackBar()
{
	LRESULT pos = SendMessageW(hTrackBar, TBM_GETPOS, 0, 0);
	wchar_t buf[4];
	wsprintfW(buf, L"%ld", pos);
	SetWindowTextW(hDataShowLbl, buf);
}

void CreateMyUpdown(HWND hwnd)
{
	INITCOMMONCONTROLSEX exs = { 0 };
	exs.dwSize = sizeof(INITCOMMONCONTROLSEX);
	exs.dwICC = ICC_UPDOWN_CLASS;
	InitCommonControlsEx(&exs);

	hUpdown = CreateWindowW(UPDOWN_CLASS, NULL, WS_VISIBLE | WS_CHILD | UDS_SETBUDDYINT | WS_BORDER | UDS_ALIGNRIGHT,
		0, 0, 0, 0, hwnd, (HMENU)ID_MY_UP_DOWN, NULL, NULL);

	hUpdownEdit = CreateWindowExW(WS_EX_CLIENTEDGE, WC_EDIT, NULL, WS_CHILD | WS_VISIBLE | ES_RIGHT, 300, 360, 75, 25, hwnd, (HMENU)ID_MY_UP_EDIT, NULL, NULL);
	hUpdownLabel = CreateWindowW(WC_STATIC, L"0", WS_CHILD | WS_VISIBLE | SS_LEFT, 300, 400, 100, 26, hwnd, (HMENU)ID_MY_UPDOWN_LABLE, NULL, NULL);

	SendMessage(hUpdown, UDM_SETBUDDY, (WPARAM)hUpdownEdit, 0);
	SendMessage(hUpdown, UDM_SETRANGE, 0, MAKELPARAM(UD_MAX_POS, UD_MIN_POS));
	SendMessage(hUpdown, UDM_SETPOS32, 0, 0);
}

void UpdateMyUpdown()
{

}