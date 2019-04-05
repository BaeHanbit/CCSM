#include <windows.h>
#include <TCHAR.H>
#include <stdio.h>
#include "resource.h"
#include <list>
#include <numeric>
#include <vector>
#include <iostream>
#include <io.h>
#include <string>

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT hScrollMove(HWND hWnd, WPARAM wParam, LPARAM lParam, int boxHeight, int s);
LRESULT CALLBACK DrawGraph_1(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam, HDC hdc);
BOOL CALLBACK Dlg_AddDay(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Dlg_DayHistory(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Dlg_DeleteCategory(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
void DestroyButton();
void DrawLine(HDC hdc, RECT rectView);
HINSTANCE hInst;
// 메인 = 0, 카테고리 = 1, 통계 = 2, < = 3, > = 4, Day = 5, edit = 6, dayAdd = 7, 카테고리 상세 기록 = 8
// 카테고리 콤보박스 = 9, 카테고리 추가 다이얼로그 = 10

int status = 0;
HWND hButton1, hButton2, h3, h4, ccsm, editButton, dayAdd, category_His, add_category;
HWND cate_combo;
HWND scroll, category_page;
HWND hwnd, dlg;
int TempPos, height = 0;
HFONT hFont;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS WndClass;
	hInst = hInstance;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("CCSM");
	RegisterClass(&WndClass);
	hwnd = CreateWindow(_T("CCSM"), _T("CCSM"), WS_POPUP, GetSystemMetrics(SM_CXSCREEN) * 0.05, GetSystemMetrics(SM_CYSCREEN) * 0.05,
		GetSystemMetrics(SM_CXSCREEN) * 0.90, GetSystemMetrics(SM_CYSCREEN) * 0.90, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static RECT rectView;
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	int boxHeight = rectView.bottom * 0.1;
	int categoryBox = rectView.bottom * 0.12;
	int box_num = 15;
	int cateHis_num = 15;
	std::string category_name[] = { "dae", "daedae", "daedaedae", "boo", "booboo", "boobooboo" };
	int cate_num = 6;
	TCHAR boxNum[5];
	short zDelta;
	//.c_str()

	switch (iMsg)
	{
	case WM_CREATE:
		GetClientRect(hwnd, &rectView);
		hButton1 = CreateWindow(TEXT("button"), TEXT("카테고리"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectView.right * 0.8, rectView.bottom * 0.0525, rectView.right * 0.2, rectView.bottom * 0.4, hwnd, (HMENU)1, hInst, NULL);
		hButton2 = CreateWindow(TEXT("button"), TEXT("통계"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectView.right * 0.8, rectView.bottom * 0.46 - 2, rectView.right * 0.2, rectView.bottom * 0.39, hwnd, (HMENU)2, hInst, NULL);
		h3 = CreateWindow(TEXT("button"), TEXT("<"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectView.right * 0.8, rectView.bottom * 0.85, rectView.right * 0.1, rectView.bottom * 0.15, hwnd, (HMENU)3, hInst, NULL);
		h4 = CreateWindow(TEXT("button"), TEXT(">"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectView.right * 0.9, rectView.bottom * 0.85, rectView.right * 0.1, rectView.bottom * 0.15, hwnd, (HMENU)4, hInst, NULL);
		ccsm = CreateWindowEx(WS_EX_TOPMOST, TEXT("button"), TEXT("Computer Can Save Money"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, rectView.right, rectView.bottom * 0.05, hwnd, (HMENU)0, hInst, NULL);
		dayAdd = CreateWindow(TEXT("button"), TEXT("+"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, (rectView.right - rectView.right * 0.2) / 7 * 6, (rectView.bottom - rectView.bottom * 0.05) / 6 * 5 + (rectView.bottom * 0.05), (rectView.right - rectView.right * 0.2) / 7, (rectView.bottom - rectView.bottom * 0.05) / 6, hwnd, (HMENU)7, hInst, NULL);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 0:
		{
			DestroyButton();
			status = 0;
			InvalidateRgn(hwnd, NULL, TRUE);
			hButton1 = CreateWindow(TEXT("button"), TEXT("카테고리"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectView.right * 0.8, rectView.bottom * 0.0525, rectView.right * 0.2, rectView.bottom * 0.4, hwnd, (HMENU)1, hInst, NULL);
			hButton2 = CreateWindow(TEXT("button"), TEXT("통계"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectView.right * 0.8, rectView.bottom * 0.46 - 2, rectView.right * 0.2, rectView.bottom * 0.39, hwnd, (HMENU)2, hInst, NULL);
			h3 = CreateWindow(TEXT("button"), TEXT("<"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectView.right * 0.8, rectView.bottom * 0.85, rectView.right * 0.1, rectView.bottom * 0.15, hwnd, (HMENU)3, hInst, NULL);
			h4 = CreateWindow(TEXT("button"), TEXT(">"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectView.right * 0.9, rectView.bottom * 0.85, rectView.right * 0.1, rectView.bottom * 0.15, hwnd, (HMENU)4, hInst, NULL);
			dayAdd = CreateWindow(TEXT("button"), TEXT("+"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, (rectView.right - rectView.right * 0.2) / 7 * 6, (rectView.bottom - rectView.bottom * 0.05) / 6 * 5 + (rectView.bottom * 0.05), (rectView.right - rectView.right * 0.2) / 7, (rectView.bottom - rectView.bottom * 0.05) / 6, hwnd, (HMENU)7, hInst, NULL);
			break;
		}
		case 1:
		{
			height = 0;
			DestroyButton();
			status = 1;
			hButton1 = CreateWindow(TEXT("button"), TEXT("메인페이지"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectView.right * 0.8, rectView.bottom * 0.0525, rectView.right * 0.2, rectView.bottom * 0.4, hwnd, (HMENU)0, hInst, NULL);
			hButton2 = CreateWindow(TEXT("button"), TEXT("통계"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectView.right * 0.8, rectView.bottom * 0.46 - 2, rectView.right * 0.2, rectView.bottom * 0.39, hwnd, (HMENU)2, hInst, NULL);
			category_His = CreateWindow(TEXT("button"), TEXT("카테고리\n\n상세보기"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE, rectView.right * 0.8, rectView.bottom * 0.85, rectView.right * 0.1, rectView.bottom * 0.15, hwnd, (HMENU)8, hInst, NULL);
			add_category = CreateWindow(TEXT("button"), TEXT("카테고리\n\n추가하기"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE, rectView.right * 0.9, rectView.bottom * 0.85, rectView.right * 0.1, rectView.bottom * 0.15, hwnd, (HMENU)10, hInst, NULL);
			scroll = CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | SBS_VERT, rectView.right * 0.78, rectView.bottom * 0.05, rectView.right * 0.02, rectView.bottom - rectView.bottom * 0.05, hwnd, NULL, hInst, NULL);
			SetScrollRange(scroll, SB_CTL, 0, categoryBox * (cateHis_num - 7), TRUE);
			SetScrollPos(scroll, SB_CTL, 0, TRUE);
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		}
		case 2:
			DestroyButton();
			status = 2;
			InvalidateRgn(hwnd, NULL, TRUE);
			hButton1 = CreateWindow(TEXT("button"), TEXT("카테고리"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectView.right * 0.8, rectView.bottom * 0.0525, rectView.right * 0.2, rectView.bottom * 0.4, hwnd, (HMENU)1, hInst, NULL);
			hButton2 = CreateWindow(TEXT("button"), TEXT("메인 페이지"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectView.right * 0.8, rectView.bottom * 0.46 - 2, rectView.right * 0.2, rectView.bottom * 0.39, hwnd, (HMENU)0, hInst, NULL);
			h3 = CreateWindow(TEXT("button"), TEXT("<"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectView.right * 0.8, rectView.bottom * 0.85, rectView.right * 0.1, rectView.bottom * 0.15, hwnd, (HMENU)3, hInst, NULL);
			h4 = CreateWindow(TEXT("button"), TEXT(">"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectView.right * 0.9, rectView.bottom * 0.85, rectView.right * 0.1, rectView.bottom * 0.15, hwnd, (HMENU)4, hInst, NULL);
			break;
		case 5:
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		case 4:
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		case 3:
			DestroyButton();
			height = 0;
			status = 5;
			scroll = CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | SBS_VERT, rectView.right * 0.98, rectView.bottom * 0.05, rectView.right * 0.02, rectView.bottom - rectView.bottom * 0.05, hwnd, NULL, hInst, NULL);
			editButton = CreateWindow(TEXT("button"), TEXT("삭제"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectView.right * 0.83, rectView.bottom * 0.89, rectView.right * 0.1, rectView.bottom * 0.07, hwnd, (HMENU)6, hInst, NULL);
			SetScrollRange(scroll, SB_CTL, 0, boxHeight * (box_num - 8), TRUE);
			SetScrollPos(scroll, SB_CTL, 0, TRUE);
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		case 6:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_DELETE_DAY_HISTORY), hwnd, (DLGPROC)Dlg_DayHistory);
			break;
		case 7:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_ADDDAY), hwnd, (DLGPROC)Dlg_AddDay);
			break;
		case 8:
			DestroyButton();
			height = 0;
			status = 8;
			scroll = CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | SBS_VERT, rectView.right * 0.98, rectView.bottom * 0.05, rectView.right * 0.02, rectView.bottom - rectView.bottom * 0.05, hwnd, NULL, hInst, NULL);
			cate_combo = CreateWindow(TEXT("combobox"), NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, rectView.right * 0.12, rectView.bottom * 0.91, rectView.right * 0.13, rectView.bottom * 0.2, hwnd, (HMENU)9, hInst, NULL);
			for (int i = 0; i < cate_num; i++)
			{
				SendMessage(cate_combo, CB_ADDSTRING, NULL, (LPARAM)category_name[i].c_str());
			}
			SendMessage(cate_combo, CB_SETCURSEL, 0, 0);
			InvalidateRgn(hwnd, NULL, TRUE);
			SetScrollRange(scroll, SB_CTL, 0, boxHeight * (cateHis_num - 8), TRUE);
			SetScrollPos(scroll, SB_CTL, 0, TRUE);
			break;
		case 10:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_ADD_DELETE_CATEGORY), hwnd, (DLGPROC)Dlg_DeleteCategory);
			break;
		}
	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);

		switch (status)
		{
		case 0:
		{
			DrawLine(hdc, rectView);
			LONG s1 = (rectView.bottom - rectView.bottom * 0.05) / 6;
			LONG s2 = (rectView.right - rectView.right * 0.2) / 7;

			for (int i = 1; i <= 5; i++)
			{
				MoveToEx(hdc, 0, s1 * i + rectView.bottom * 0.05, NULL);
				LineTo(hdc, rectView.right * 0.8 - 1, s1 * i + rectView.bottom * 0.05);
			}
			for (int i = 1; i <= 6; i++)
			{
				MoveToEx(hdc, s2 * i, rectView.bottom * 0.05, NULL);
				LineTo(hdc, s2 * i, rectView.bottom);
			}
			break;
		}
		case 1:
		{
			DrawLine(hdc, rectView);
			hBrush = CreateSolidBrush(RGB(250, 243, 219));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			for (int i = 1; i <= cateHis_num; i++)
			{
				if (i == 1)
				{
					Rectangle(hdc, 0, rectView.bottom * 0.05 - height, rectView.right *  0.78, rectView.bottom * 0.05 + categoryBox - height);
					SetBkMode(hdc, TRANSPARENT);
					TextOut(hdc, rectView.right * 0.06, rectView.bottom * 0.025 + categoryBox / 2 - height, "[식비]", _tcslen("[식비]"));
					TextOut(hdc, rectView.right * 0.03, rectView.bottom * 0.06 + categoryBox / 2 - height, "[70%] 3500 / 7000", _tcslen("[70%] 3500 / 7000"));
				}
				else
				{
					Rectangle(hdc, 0, rectView.bottom * 0.05 + categoryBox * (i - 1) - height, rectView.right *  0.78, rectView.bottom * 0.05 + categoryBox * i - height);
					SetBkMode(hdc, TRANSPARENT);
					TextOut(hdc, rectView.right * 0.06, rectView.bottom * 0.025 + categoryBox * (i - 1) + categoryBox / 2 - height, "[기타]", _tcslen("[기타]"));
					TextOut(hdc, rectView.right * 0.03, rectView.bottom * 0.06 + categoryBox * (i - 1) + categoryBox / 2 - height, "[70%] 3500 / 7000", _tcslen("[70%] 3500 / 7000"));
				}
			}

			hBrush = CreateSolidBrush(RGB(255, 255, 255));
			SelectObject(hdc, hBrush);

			for (int i = 1; i <= cateHis_num; i++)
			{
				if (i == 1)
				{
					Rectangle(hdc, rectView.right * 0.15, rectView.bottom * 0.08 - height, rectView.right * 0.75, rectView.bottom * 0.14 - height);
				}
				else
				{
					Rectangle(hdc, rectView.right * 0.15, rectView.bottom * 0.08 + categoryBox * (i - 1) - height, rectView.right * 0.75, rectView.bottom * 0.14 + categoryBox * (i - 1) - height);
				}
			}

			hBrush = CreateSolidBrush(RGB(133, 133, 133));
			SelectObject(hdc, hBrush);

			for (int i = 1; i <= cateHis_num; i++)
			{
				if (i == 1)
				{
					Rectangle(hdc, rectView.right * 0.15, rectView.bottom * 0.08 - height, rectView.right * 0.6, rectView.bottom * 0.14 - height);
				}
				else
				{
					Rectangle(hdc, rectView.right * 0.15, rectView.bottom * 0.08 + categoryBox * (i - 1) - height, rectView.right * 0.25 + rectView.right * 0.5 / i, rectView.bottom * 0.14 + categoryBox * (i - 1) - height);
				}
			}


			hBrush = CreateSolidBrush(RGB(223, 230, 247));
			SelectObject(hdc, hBrush);

			Rectangle(hdc, 0, rectView.bottom * 0.88, rectView.right *  0.78, rectView.bottom);
			TextOut(hdc, rectView.right * 0.06, rectView.bottom * 0.835 + rectView.bottom * 0.08, "[Total]", _tcslen("[Total]"));
			TextOut(hdc, rectView.right * 0.03, rectView.bottom * 0.89 + categoryBox / 2, "[70%] 3500 / 7000", _tcslen("[70%] 3500 / 7000"));

			hBrush = CreateSolidBrush(RGB(255, 255, 255));
			SelectObject(hdc, hBrush);
			Rectangle(hdc, rectView.right * 0.15, rectView.bottom * 0.91, rectView.right *  0.75, rectView.bottom * 0.97);

			hBrush = CreateSolidBrush(RGB(133, 133, 133));
			SelectObject(hdc, hBrush);
			Rectangle(hdc, rectView.right * 0.15, rectView.bottom * 0.91, rectView.right * 0.55, rectView.bottom * 0.97);

			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
			break;
		}
		case 2:
			DrawLine(hdc, rectView);
			break;
		case 3:
			break;
		case 5:
			hBrush = CreateSolidBrush(RGB(250, 243, 219));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			for (int i = 1; i <= box_num; i++)
			{
				if (i == 1)
				{
					Rectangle(hdc, 0, rectView.bottom * 0.05 - height, rectView.right *  0.98, rectView.bottom * 0.05 + boxHeight - height);
					SetBkMode(hdc, TRANSPARENT);
					wsprintf(boxNum, TEXT("%d"), i);
					TextOut(hdc, rectView.right * 0.05, rectView.bottom * 0.04 + boxHeight / 2 - height, boxNum, _tcslen(boxNum));
					TextOut(hdc, rectView.right * 0.15, rectView.bottom * 0.04 + boxHeight / 2 - height, "[07 : 00]", _tcslen("[07 : 00]"));
					TextOut(hdc, rectView.right * 0.35, rectView.bottom * 0.04 + boxHeight / 2 - height, "[+ 1500\]", _tcslen("[+ 1500\]"));
					TextOut(hdc, rectView.right * 0.55, rectView.bottom * 0.04 + boxHeight / 2 - height, "[전체]", _tcslen("[전체]"));
					TextOut(hdc, rectView.right * 0.75, rectView.bottom * 0.04 + boxHeight / 2 - height, "[Pay Back]", _tcslen("[Pay Back]"));
				}
				else
				{
					Rectangle(hdc, 0, rectView.bottom * 0.05 + boxHeight * (i - 1) - height, rectView.right *  0.98, rectView.bottom * 0.05 + boxHeight * i - height);
					SetBkMode(hdc, TRANSPARENT);
					wsprintf(boxNum, TEXT("%d"), i);
					TextOut(hdc, rectView.right * 0.05, rectView.bottom * 0.04 + boxHeight * (i - 1) + boxHeight / 2 - height, boxNum, _tcslen(boxNum));
					TextOut(hdc, rectView.right * 0.15, rectView.bottom * 0.04 + boxHeight * (i - 1) + boxHeight / 2 - height, "[07 : 00]", _tcslen("[07 : 00]"));
					TextOut(hdc, rectView.right * 0.35, rectView.bottom * 0.04 + boxHeight * (i - 1) + boxHeight / 2 - height, "[+ 1500\]", _tcslen("[+ 1500\]"));
					TextOut(hdc, rectView.right * 0.55, rectView.bottom * 0.04 + boxHeight * (i - 1) + boxHeight / 2 - height, "[전체]", _tcslen("[전체]"));
					TextOut(hdc, rectView.right * 0.75, rectView.bottom * 0.04 + boxHeight * (i - 1) + boxHeight / 2 - height, "[Pay Back]", _tcslen("[Pay Back]"));
				}
			}
			hBrush = CreateSolidBrush(RGB(133, 133, 133));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			
			Rectangle(hdc, 0, rectView.bottom * 0.85, rectView.right *  0.98, rectView.bottom);
			TextOut(hdc, rectView.right * 0.15, rectView.bottom * 0.85 + rectView.bottom * 0.07, "[07 : 00]", _tcslen("[07 : 00]"));
			TextOut(hdc, rectView.right * 0.4, rectView.bottom * 0.85 + rectView.bottom * 0.07, "[전체]", _tcslen("[전체]"));
			TextOut(hdc, rectView.right * 0.65, rectView.bottom * 0.85 + rectView.bottom * 0.07, "[Pay Back]", _tcslen("[Pay Back]"));
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
			break;
		case 8:
			hBrush = CreateSolidBrush(RGB(250, 243, 219));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			for (int i = 1; i <= cateHis_num; i++)
			{
				if (i == 1)
				{
					Rectangle(hdc, 0, rectView.bottom * 0.05 - height, rectView.right *  0.98, rectView.bottom * 0.05 + boxHeight - height);
					SetBkMode(hdc, TRANSPARENT);
					TextOut(hdc, rectView.right * 0.12, rectView.bottom * 0.04 + boxHeight / 2 - height, "< 2019-01-23 22:11 >", _tcslen("< 2019-01-23 22:11 >"));
					TextOut(hdc, rectView.right * 0.35, rectView.bottom * 0.04 + boxHeight / 2 - height, "\ 1500", _tcslen("\ 1500"));
					TextOut(hdc, rectView.right * 0.55, rectView.bottom * 0.04 + boxHeight / 2 - height, "[지출]", _tcslen("[지출]"));
					TextOut(hdc, rectView.right * 0.75, rectView.bottom * 0.04 + boxHeight / 2 - height, "[Pay Back]", _tcslen("[Pay Back]"));
				}
				else
				{
					Rectangle(hdc, 0, rectView.bottom * 0.05 + boxHeight * (i - 1) - height, rectView.right *  0.98, rectView.bottom * 0.05 + boxHeight * i - height);
					SetBkMode(hdc, TRANSPARENT);
					TextOut(hdc, rectView.right * 0.12, rectView.bottom * 0.04 + boxHeight * (i - 1) + boxHeight / 2 - height, "< 2019-01-23 22:11 >", _tcslen("< 2019-01-23 22:11 >"));
					TextOut(hdc, rectView.right * 0.35, rectView.bottom * 0.04 + boxHeight * (i - 1) + boxHeight / 2 - height, "\ 1500", _tcslen("\ 1500"));
					TextOut(hdc, rectView.right * 0.55, rectView.bottom * 0.04 + boxHeight * (i - 1) + boxHeight / 2 - height, "[수입]", _tcslen("[수입]"));
					TextOut(hdc, rectView.right * 0.75, rectView.bottom * 0.04 + boxHeight * (i - 1) + boxHeight / 2 - height, "[Pay Back]", _tcslen("[Pay Back]"));
				}
			}
			hBrush = CreateSolidBrush(RGB(133, 133, 133));
			SelectObject(hdc, hBrush);


			Rectangle(hdc, 0, rectView.bottom * 0.85, rectView.right *  0.98, rectView.bottom);
			TextOut(hdc, rectView.right * 0.40, rectView.bottom * 0.85 + rectView.bottom * 0.07, "전체 잔액 : 13000", _tcslen("전체 잔액 : 13000"));
			TextOut(hdc, rectView.right * 0.70, rectView.bottom * 0.85 + rectView.bottom * 0.07, "초기화 날짜 : 2019-01-01", _tcslen("초기화 날짜 : 2019-01-01"));
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
			break;
		} 
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
		{
			PostQuitMessage(0);
			return 0;
		}
		}
		break;
	}
	case WM_LBUTTONUP:
		POINT mouse;
		GetCursorPos(&mouse);
		switch (status)
		{
		case 0:
			if (mouse.x < rectView.right * 0.8 && mouse.y > rectView.bottom * 0.05)
			{
				MessageBox(hwnd, "dee", "dee", MB_OK);
				status = 5;
				SendMessage(hwnd, WM_COMMAND, NULL, NULL);
			}
			break;
		}
		break;
	case WM_VSCROLL:
		switch (status)
		{
		case 1:
			hScrollMove(hwnd, wParam, lParam, categoryBox, cateHis_num - 7);
			SetScrollPos(scroll, SB_CTL, TempPos, TRUE);
			break;
		case 5:
			hScrollMove(hwnd, wParam, lParam, boxHeight, box_num - 8);
			SetScrollPos(scroll, SB_CTL, TempPos, TRUE);
			break;
		case 8:
			hScrollMove(hwnd, wParam, lParam, boxHeight, box_num - 8);
			SetScrollPos(scroll, SB_CTL, TempPos, TRUE);
			break;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_MOUSEWHEEL:
		if (status == 1)
		{
			zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

			TempPos = height;

			if (zDelta < 0)
				TempPos = min(categoryBox * (cateHis_num - 7), height + categoryBox);
			else
				TempPos = max(0, height - categoryBox);

			height = TempPos;

			SetScrollPos(scroll, SB_CTL, TempPos, TRUE);
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		else if (status == 5)
		{
			zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

			TempPos = height;

			if (zDelta < 0)
				TempPos = min(boxHeight * (box_num - 8), height + boxHeight);
			else
				TempPos = max(0, height - boxHeight);

			height = TempPos;

			SetScrollPos(scroll, SB_CTL, TempPos, TRUE);
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		if (status == 8)
		{
			zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

			TempPos = height;

			if (zDelta < 0)
				TempPos = min(boxHeight * (cateHis_num - 8), height + boxHeight);
			else
				TempPos = max(0, height - boxHeight);

			height = TempPos;

			SetScrollPos(scroll, SB_CTL, TempPos, TRUE);
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

LRESULT hScrollMove(HWND hWnd, WPARAM wParam, LPARAM lParam, int boxHeight, int s)
{
	TempPos = height;
	switch (LOWORD(wParam))
	{
	case SB_LINELEFT:
		TempPos = max(0, TempPos - boxHeight);
		break;
	case SB_LINERIGHT:
		TempPos = min(boxHeight * s, TempPos + boxHeight);
		break;
	case SB_PAGELEFT:
		TempPos = max(0, TempPos - boxHeight);
		break;
	case SB_PAGERIGHT:
		TempPos = min(boxHeight * s, TempPos + boxHeight);
		break;
	case SB_THUMBTRACK:
		TempPos = HIWORD(wParam);
		break;
	}
	height = TempPos;
	return 0;
}

void DestroyButton()
{
	DestroyWindow(hButton1);
	DestroyWindow(hButton2);
	DestroyWindow(h3);
	DestroyWindow(h4);
	DestroyWindow(scroll);
	DestroyWindow(editButton);
	DestroyWindow(scroll);
	DestroyWindow(dayAdd);
	DestroyWindow(category_His);
	DestroyWindow(cate_combo);
	DestroyWindow(add_category);
}

void DrawLine(HDC hdc, RECT rectView)
{
	MoveToEx(hdc, rectView.right * 0.8 - 1, rectView.bottom * 0.05, NULL);
	LineTo(hdc, rectView.right * 0.8 - 1, rectView.bottom);
	MoveToEx(hdc, rectView.right * 0.8 - 1, rectView.bottom * 0.455, NULL);
	LineTo(hdc, rectView.right, rectView.bottom * 0.455);
	MoveToEx(hdc, rectView.right * 0.8 - 1, rectView.bottom * 0.847, NULL);
	LineTo(hdc, rectView.right, rectView.bottom * 0.847);
}

BOOL CALLBACK Dlg_AddDay(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hCombo;
	char hArr[][12] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12" };
	char mArr[][60] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12",
		"13", "14", "15" ,"16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31",
		"32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50",
		"51", "52", "53", "54", "55", "56", "57", "58", "59" };

	switch (iMsg)
	{
	case WM_INITDIALOG:
		SetDlgItemInt(hDlg, IDC_EDIT_YEAR, 2019, TRUE);
		SetDlgItemInt(hDlg, IDC_EDIT_MONTH, 3, TRUE);
		SetDlgItemInt(hDlg, IDC_EDIT_DAY, 28, TRUE);

		hCombo = GetDlgItem(hDlg, IDC_COMBO_HOUR);
		for (int i = 0; i < 12; i++)
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)hArr[i]);
		hCombo = GetDlgItem(hDlg, IDC_COMBO_MINUTE);
		for (int i = 0; i <= 59; i++)
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)mArr[i]);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			MessageBox(hDlg, "Ho", "Ho", MB_OK);
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0;
}

BOOL CALLBACK Dlg_DayHistory(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
	}
	return 0;
}

BOOL CALLBACK Dlg_DeleteCategory(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
	}
	return 0;
}

LRESULT CALLBACK DrawGraph_1(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam, HDC hdc)
{


	return 0;
}