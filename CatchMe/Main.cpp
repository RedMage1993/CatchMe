//********************************************************
// Author: Fritz Ammon
// Date: 17 July 2011
//********************************************************

#pragma comment (linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment (lib, "Comctl32")
#pragma comment (lib, "Winmm")

#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <CommCtrl.h>
#include "resource.h"

using namespace std;

DWORD WINAPI ThreadProcess(LPVOID lpParameter);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

const int WND_WIDTH = 320;
const int WND_HEIGHT = 240;

HFONT g_hFont = 0;
HINSTANCE g_hInstance = 0;
HWND g_hWnd = 0;
BOOL g_bIsClosing = FALSE;
int maxx = GetSystemMetrics(0);
int maxy = GetSystemMetrics(1);

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	INITCOMMONCONTROLSEX iccx;
	LOGFONT lf;
	TCHAR lpszClassName[] = TEXT("NiggehPoopie");
	WNDCLASSEX wcx;
	HWND hWnd = 0;
	MSG msg;
	BOOL bRet = FALSE;
	HANDLE hThreadProcess = 0;

	iccx.dwICC =  ICC_STANDARD_CLASSES;
	iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);

	if (!InitCommonControlsEx(&iccx))
		goto exit;

	SecureZeroMemory(reinterpret_cast<PVOID> (&lf), sizeof(lf));
	lf.lfHeight = -22;
	wcscpy_s(lf.lfFaceName, TEXT("Comic Sans MS"));
	lf.lfWeight = FW_NORMAL;

	if ((g_hFont = CreateFontIndirect(&lf)) == 0)
		goto exit;

	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WindowProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = reinterpret_cast<HBRUSH> (COLOR_BTNFACE + 1);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = lpszClassName;
	wcx.hIconSm = NULL;

	if (!RegisterClassEx(&wcx))
		goto exit;

	hWnd = CreateWindowEx(
		WS_EX_TOPMOST,
		lpszClassName,
		TEXT("CatchMe"),
		WS_POPUP,
		maxx / 2 - 320 / 2,
		maxy / 2 - 240 / 2,
		WND_WIDTH,
		WND_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (hWnd == 0)
		goto exit;

	if ((hThreadProcess = CreateThread(NULL, 0, ThreadProcess, NULL, 0, NULL)) == 0)
		goto exit;

	ShowWindow(hWnd, nCmdShow);
	if (!UpdateWindow(hWnd))
		goto exit;

	g_hInstance = hInstance;
	g_hWnd = hWnd;

	srand(static_cast<unsigned int> (time(0)));

	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
			break;
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

exit:
	CloseHandle(hThreadProcess);

	return msg.wParam;
}

DWORD WINAPI ThreadProcess(LPVOID lpParameter)
{
	POINT ptMouse;
	RECT rctWnd;
	int nDist = 0;

	do
	{
		GetCursorPos(&ptMouse);
		GetWindowRect(g_hWnd, &rctWnd);

		if (PtInRect(&rctWnd, ptMouse))
		{
			switch ((1 + rand()) % (6) + 1)
			{
			case 1:
				PlaySound(MAKEINTRESOURCE(IDR_WAVE1), g_hInstance, SND_ASYNC | SND_RESOURCE);
				break;

			case 2:
				PlaySound(MAKEINTRESOURCE(IDR_WAVE2), g_hInstance, SND_ASYNC | SND_RESOURCE);
				break;

			case 3:
				PlaySound(MAKEINTRESOURCE(IDR_WAVE3), g_hInstance, SND_ASYNC | SND_RESOURCE);
				break;

			case 4:
				PlaySound(MAKEINTRESOURCE(IDR_WAVE4), g_hInstance, SND_ASYNC | SND_RESOURCE);
				break;

			case 5:
				PlaySound(MAKEINTRESOURCE(IDR_WAVE5), g_hInstance, SND_ASYNC | SND_RESOURCE);
				break;

			case 6:
				PlaySound(MAKEINTRESOURCE(IDR_WAVE6), g_hInstance, SND_ASYNC | SND_RESOURCE);
				break;

			}

			if (ptMouse.x <= rctWnd.left + 80)
			{
				nDist = rctWnd.left + 160 - ptMouse.x; // width of button

				rctWnd.left += nDist / 2;

				SetWindowPos(g_hWnd, NULL, rctWnd.left, rctWnd.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			}
			else if (ptMouse.x >= rctWnd.right - 80)
			{
				nDist = ptMouse.x - rctWnd.right - 160; // width of button

				rctWnd.left += nDist / 2;

				SetWindowPos(g_hWnd, NULL, rctWnd.left, rctWnd.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			}

			if (ptMouse.y <= rctWnd.top + 60)
			{
				nDist = rctWnd.top + 120 - ptMouse.y; // height of button

				rctWnd.top += nDist / 2;

				SetWindowPos(g_hWnd, NULL, rctWnd.left, rctWnd.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			}
			else if (ptMouse.y >= rctWnd.bottom - 60)
			{
				nDist = ptMouse.y - rctWnd.bottom - 120; // height of button

				rctWnd.top += nDist / 2;

				SetWindowPos(g_hWnd, NULL, rctWnd.left, rctWnd.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			}

			//out of range
			if (rctWnd.left < 0)
				SetWindowPos(g_hWnd, NULL, rctWnd.left + 320, rctWnd.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			else if (rctWnd.right > maxx)
				SetWindowPos(g_hWnd, NULL, rctWnd.left - 320, rctWnd.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

			if (rctWnd.top < 0)
				SetWindowPos(g_hWnd, NULL, rctWnd.left, rctWnd.top + 240, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			else if (rctWnd.bottom > maxy)
				SetWindowPos(g_hWnd, NULL, rctWnd.left, rctWnd.top - 240, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}

		Sleep(5);
	} while (!g_bIsClosing);

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hchild = 0;

	switch (uMsg)
	{
	case WM_CREATE:
		hchild = CreateWindowEx(NULL, WC_BUTTON, TEXT("Catch me!"),
			WS_CHILD | WS_VISIBLE,
			320 / 2 - 160 / 2, 240 / 2 - 120 / 2, 160, 120,
			hwnd, reinterpret_cast<HMENU> (1), g_hInstance, NULL);
		SendMessage(hchild, WM_SETFONT, reinterpret_cast<WPARAM> (g_hFont), MAKELPARAM(TRUE, NULL));
		break;

	case WM_DESTROY:
		g_bIsClosing = TRUE;
		PlaySound(MAKEINTRESOURCE(IDR_WAVE7), g_hInstance, SND_ASYNC | SND_RESOURCE);

		MessageBox(hwnd, TEXT("D: ...a hero died today"), TEXT("cruel world...:("), MB_OK);

		PostQuitMessage(0);
		break;

	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case BN_CLICKED:
			switch (LOWORD(wParam))
			{
			case 1:
				DestroyWindow(hwnd);
				break;

			}
			break;

		}
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}