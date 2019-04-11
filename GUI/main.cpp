#pragma once
#include <windows.h>
#include <TCHAR.H>
#include <stdio.h>
#include "resource.h"
#include <list>
#include <numeric>
#include <vector>
#include <iostream>
#include <io.h>
#include <map>
#include <string>
#include "file.h"
#include "function.h"

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

extern std::map<std::string, weekday> calender;

std::string current_date = __today_date();
std::string today_year_s = current_date.substr(0, 4);
std::string today_month_s = current_date.substr(5, 2);
std::string today_day_s = current_date.substr(8, 2);
int today_year_i = std::stoi(today_year_s);
int today_month_i = std::stoi(today_month_s);
int today_day_i = std::stoi(today_day_s);
int selected_year = today_year_i;
int selected_month = today_month_i;
int selected_month_day = __Maximum_day(selected_month, selected_year);

int cate_num = 0;
std::string category_name[20];

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
	TCHAR boxNum[5];
	TCHAR category_percent[20];
	short zDelta;
	int category_income, category_expense, category_total_income = 0, category_total_expense = 0;

	switch (iMsg)
	{
	case WM_CREATE:
		__Create_dir(CATEGORY);
		__Create_dir(MASTER);
		__Create_index_file();
		__Set_calender();
		cate_num = __Get_all_file(category_name, MASTER);
		for (int i = 0; i < cate_num; i++)
		{
			for (int j = 0; j < 4; j++)
				category_name[i].pop_back();
		}
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
			selected_year = today_year_i;
			selected_month = today_month_i;
			selected_month_day = __Maximum_day(selected_month, selected_year);
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
			selected_year = today_year_i;
			selected_month = today_month_i;
			height = 0;
			DestroyButton();
			status = 1;
			cate_num = __Get_all_file(category_name, MASTER);
			for (int i = 0; i < cate_num; i++)
			{
				for(int j = 0; j < 4; j++)
					category_name[i].pop_back();
			}
			hButton1 = CreateWindow(TEXT("button"), TEXT("메인페이지"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectView.right * 0.8, rectView.bottom * 0.0525, rectView.right * 0.2, rectView.bottom * 0.4, hwnd, (HMENU)0, hInst, NULL);
			hButton2 = CreateWindow(TEXT("button"), TEXT("통계"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectView.right * 0.8, rectView.bottom * 0.46 - 2, rectView.right * 0.2, rectView.bottom * 0.39, hwnd, (HMENU)2, hInst, NULL);
			category_His = CreateWindow(TEXT("button"), TEXT("카테고리\n\n상세보기"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE, rectView.right * 0.8, rectView.bottom * 0.85, rectView.right * 0.1, rectView.bottom * 0.15, hwnd, (HMENU)8, hInst, NULL);
			add_category = CreateWindow(TEXT("button"), TEXT("카테고리\n\n추가하기"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE, rectView.right * 0.9, rectView.bottom * 0.85, rectView.right * 0.1, rectView.bottom * 0.15, hwnd, (HMENU)10, hInst, NULL);
			scroll = CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | SBS_VERT, rectView.right * 0.78, rectView.bottom * 0.05, rectView.right * 0.02, rectView.bottom - rectView.bottom * 0.05, hwnd, NULL, hInst, NULL);
			if (cate_num > 7)
				SetScrollRange(scroll, SB_CTL, 0, categoryBox * (cate_num - 7), TRUE);
			else
				SetScrollRange(scroll, SB_CTL, 0, 0, TRUE);
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
		case 3:
			switch (status)
			{
			case 0:
				if (selected_month - 1 == 0)
				{
					selected_month = 12;
					selected_year -= 1;
					selected_month_day = __Maximum_day(selected_month, selected_year);
				}
				else
				{
					selected_month -= 1;
					selected_month_day = __Maximum_day(selected_month, selected_year);
				}
				break;
			}
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		case 4:
			switch (status)
			{
			case 0:
				if (selected_month + 1 == 13)
				{
					selected_month = 1;
					selected_year += 1;
					selected_month_day = __Maximum_day(selected_month, selected_year);
				}
				else
				{
					selected_month += 1;
					selected_month_day = __Maximum_day(selected_month, selected_year);
				}
			}
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		case 5:
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
			SendMessage(hwnd, WM_COMMAND, 1, NULL);
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
			int flag = 0;
			int n = 1;
			for (int i = 1; i <= 6; i++)
			{
				for (int j = 1; j <= 7; j++)
				{
					if (n > selected_month_day)
						break;
					if (flag == 0)
					{
						std::string sele_month = "";
						if (selected_month < 10)
							sele_month = "0" + std::to_string(selected_month);
						else
							sele_month = std::to_string(selected_month);

						std::string calender_data;
						calender_data = std::to_string(selected_year) + "-" + sele_month + "-01";

						if (calender.find(calender_data)->second == j - 1)
							flag = 1;
					}
					if (flag == 1)
					{
						TextOut(hdc, s2 * j - s2 * 0.93, s1 * i - s1 * 0.5, std::to_string(n).c_str(), _tcslen(std::to_string(n).c_str()));
						n++;
					}
				}
			}
			break;
		}
		case 1:
		{
			DrawLine(hdc, rectView);
			hBrush = CreateSolidBrush(RGB(250, 243, 219));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			double percent;
			int i_percent;

			for (int i = 1; i <= cate_num; i++)
			{
				hBrush = CreateSolidBrush(RGB(250, 243, 219));	
				SelectObject(hdc, hBrush);

				std::string selected_m;

				if (selected_month < 10)
					selected_m = '0' + std::to_string(selected_month);
				else
					selected_m = std::to_string(selected_month);
				category_income = __Get_total_income(category_name[i - 1], std::to_string(selected_year) + '-' + selected_m);
				category_expense = __Get_total_expense(category_name[i - 1], std::to_string(selected_year) + '-' + selected_m);

				category_total_income += category_income;
				category_total_expense += category_expense;

				if (category_income == 0 && category_expense == 0)
					percent = 0;
				else if (category_expense == 0)
					percent = 1;
				else 
					percent = (double)category_expense / category_income;

				i_percent = percent * 100;

				if (percent > 1)
				{
					wsprintf(category_percent, TEXT("[%d%%] %d / %d"), 100, category_income - category_expense, category_income);
				}
				else
				{
					wsprintf(category_percent, TEXT("[%d%%] %d / %d"), i_percent, category_income - category_expense, category_income);
				}

				if (i == 1)
				{
					Rectangle(hdc, 0, rectView.bottom * 0.05 - height, rectView.right *  0.78, rectView.bottom * 0.05 + categoryBox - height);
					SetBkMode(hdc, TRANSPARENT);
					TextOut(hdc, rectView.right * 0.05, rectView.bottom * 0.025 + categoryBox / 2 - height, category_name[i - 1].c_str(), _tcslen(category_name[i - 1].c_str()));
					TextOut(hdc, rectView.right * 0.03, rectView.bottom * 0.06 + categoryBox / 2 - height, category_percent, _tcslen(category_percent));
				}
				else
				{
					Rectangle(hdc, 0, rectView.bottom * 0.05 + categoryBox * (i - 1) - height, rectView.right *  0.78, rectView.bottom * 0.05 + categoryBox * i - height);
					SetBkMode(hdc, TRANSPARENT);
					TextOut(hdc, rectView.right * 0.05, rectView.bottom * 0.025 + categoryBox * (i - 1) + categoryBox / 2 - height, category_name[i - 1].c_str(), _tcslen(category_name[i - 1].c_str()));
					TextOut(hdc, rectView.right * 0.03, rectView.bottom * 0.06 + categoryBox * (i - 1) + categoryBox / 2 - height, category_percent, _tcslen(category_percent));
				}

				hBrush = CreateSolidBrush(RGB(255, 255, 255));
				SelectObject(hdc, hBrush);

				if (i == 1)
				{
					Rectangle(hdc, rectView.right * 0.15, rectView.bottom * 0.08 - height, rectView.right * 0.75, rectView.bottom * 0.14 - height);
				}
				else
				{
					Rectangle(hdc, rectView.right * 0.15, rectView.bottom * 0.08 + categoryBox * (i - 1) - height, rectView.right * 0.75, rectView.bottom * 0.14 + categoryBox * (i - 1) - height);
				}

				hBrush = CreateSolidBrush(RGB(133, 133, 133));
				SelectObject(hdc, hBrush);

				if (i == 1)
				{
					if (percent >= 1)
					{
						Rectangle(hdc, rectView.right * 0.15, rectView.bottom * 0.08 - height, rectView.right * 0.75, rectView.bottom * 0.14 - height);
					}
					else
					{
						Rectangle(hdc, rectView.right * 0.15, rectView.bottom * 0.08 - height, rectView.right * 0.15 + (rectView.right * 0.6) * percent, rectView.bottom * 0.14 - height);
					}
				}
				else
				{
					if (percent >= 1)
					{
						Rectangle(hdc, rectView.right * 0.15, rectView.bottom * 0.08 + categoryBox * (i - 1) - height, rectView.right * 0.75, rectView.bottom * 0.14 + categoryBox * (i - 1) - height);
					}
					else
					{
						Rectangle(hdc, rectView.right * 0.15, rectView.bottom * 0.08 + categoryBox * (i - 1) - height, rectView.right * 0.15 + (rectView.right * 0.6) * percent, rectView.bottom * 0.14 + categoryBox * (i - 1) - height);
					}
				}
			}

			hBrush = CreateSolidBrush(RGB(223, 230, 247));
			SelectObject(hdc, hBrush);

			if (category_total_income == 0 && category_total_expense == 0)
				percent = 0;
			else if (category_total_expense == 0)
				percent = 1;
			else
				percent = (double)category_total_expense / category_total_income;

			i_percent = percent * 100;

			if (percent > 1)
			{
				wsprintf(category_percent, TEXT("[%d%%] %d / %d"), 100, category_total_income - category_total_expense, category_total_income);
			}
			else
			{
				wsprintf(category_percent, TEXT("[%d%%] %d / %d"), i_percent, category_total_income - category_total_expense, category_total_income);
			}

			Rectangle(hdc, 0, rectView.bottom * 0.88, rectView.right *  0.78, rectView.bottom);
			TextOut(hdc, rectView.right * 0.06, rectView.bottom * 0.835 + rectView.bottom * 0.08, "[Total]", _tcslen("[Total]"));
			TextOut(hdc, rectView.right * 0.03, rectView.bottom * 0.89 + categoryBox / 2, category_percent, _tcslen(category_percent));

			hBrush = CreateSolidBrush(RGB(255, 255, 255));
			SelectObject(hdc, hBrush);
			Rectangle(hdc, rectView.right * 0.15, rectView.bottom * 0.91, rectView.right *  0.75, rectView.bottom * 0.97);

			hBrush = CreateSolidBrush(RGB(133, 133, 133));
			SelectObject(hdc, hBrush);
			Rectangle(hdc, rectView.right * 0.15, rectView.bottom * 0.91, rectView.right * 0.15 + (rectView.right * 0.6) * percent, rectView.bottom * 0.97);

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

			}
			break;
		}
		break;
	case WM_VSCROLL:
		switch (status)
		{
		case 1:
			if (cate_num <= 7)
				return 1;
			hScrollMove(hwnd, wParam, lParam, categoryBox, cate_num - 7);
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
			if (cate_num <= 7)
				return 1;
			zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

			TempPos = height;

			if (zDelta < 0)
				TempPos = min(categoryBox * (cate_num - 7), height + categoryBox);
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

BOOL CALLBACK Dlg_DayHistory(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

	switch (iMsg)
	{
	case WM_INITDIALOG:
		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0;
}

BOOL CALLBACK Dlg_AddDay(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	int year = 0, month = 0, day = 0, a_day;
	std::string day_s;
	char c_name[21];
	bool ie;
	int hour = 0, minute = 0;
	std::string hour_s, minute_s;
	int money = 0;
	char memo[41];
	std::string check;
	HWND con;
	switch (iMsg)
	{
	case WM_INITDIALOG:
		SetDlgItemInt(hDlg, IDC_EDIT_YEAR, today_year_i, TRUE);
		SetDlgItemInt(hDlg, IDC_EDIT_MONTH, today_month_i, TRUE);
		SetDlgItemInt(hDlg, IDC_EDIT_DAY, today_day_i, TRUE);

		con = GetDlgItem(hDlg, IDC_RADIO_INCOME);
		if (SendMessage(con, BM_GETCHECK, 0, 0) == BST_UNCHECKED)
		{
			SendMessage(con, BM_SETCHECK, BST_CHECKED, 0);
		}

		con = GetDlgItem(hDlg, IDC_COMBO_CATEGORY);
		for (int i = 0; i < cate_num; i++)
		{
			SendMessage(con, CB_ADDSTRING, 0, (LPARAM)category_name[i].c_str());
		}
		//CheckRadioButton(hDlg, IDC_RADIO_INCOME, IDC_RADIO_EXPENSE, IDC_RADIO_EXPENSE);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			year = GetDlgItemInt(hDlg, IDC_EDIT_YEAR, NULL, TRUE);
			month = GetDlgItemInt(hDlg, IDC_EDIT_MONTH, NULL, TRUE);
			day = GetDlgItemInt(hDlg, IDC_EDIT_DAY, NULL, TRUE);
			if (year < 2000 || year > today_year_i + 1)
			{
				MessageBox(hDlg, "올바른 날짜를 입력하세요", "Error", MB_OK);
				break;
			}
			else if (month < 1 || month > 12)
			{
				MessageBox(hDlg, "올바른 날짜를 입력하세요", "Error", MB_OK);
				break;
			}
			a_day = __Maximum_day(month, year);
			if (day < 1 || day > a_day)
			{
				MessageBox(hDlg, "올바른 날짜를 입력하세요", "Error", MB_OK);
				break;
			}

			if (day < 10)
			{
				day_s = '0' + std::to_string(day);
			}

			GetDlgItemText(hDlg, IDC_COMBO_CATEGORY, c_name, 20);
			check = c_name;
			if (check == "")
			{
				MessageBox(hDlg, "카테고리 이름을 선택하세요.", "ERROR", MB_OK);
				break;
			}

			if (IsDlgButtonChecked(hDlg, IDC_RADIO_INCOME) == 1)
			{
				ie = INCOME;
			}
			else if (IsDlgButtonChecked(hDlg, IDC_RADIO_EXPENSE) == 1)
			{
				ie = EXPENSE;
			}
			else
			{
				MessageBox(hDlg, "수입, 지출 중 하나를 선택하세요.", "Error", MB_OK);
				break;
			}

			hour = GetDlgItemInt(hDlg, IDC_EDIT_HOUR, NULL, TRUE);	
			minute = GetDlgItemInt(hDlg, IDC_EDIT_MINUTE, NULL, TRUE);
			if (hour < 0 || hour > 23)
			{
				MessageBox(hDlg, "올바른 시간을 입력하세요", "Error", MB_OK);
				break;
			}
			else if (minute < 0 || minute > 59)
			{
				MessageBox(hDlg, "올바른 시간을 입력하세요", "Error", MB_OK);
				break;
			}

			if (hour < 10)
			{
				hour_s = '0' + std::to_string(hour);
			}
			
			if (minute < 10)
			{
				minute_s = '0' + std::to_string(minute);
			}

			money = GetDlgItemInt(hDlg, IDC_EDIT_MONEY, NULL, TRUE);
			if (money < 1)
			{
				MessageBox(hDlg, "올바른 금액을 입력하세요", "Error", MB_OK);
				break;
			}
			GetDlgItemText(hDlg, IDC_EDIT_MEMO, memo, 40);

			__Insert_cate_data(c_name, std::to_string(year) + '-' + std::to_string(month) + '-' + day_s + '-' + hour_s + '-' + minute_s, memo, money, ie);
			EndDialog(hDlg, 0);
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		} 
	}
	return 0;
}

BOOL CALLBACK Dlg_DeleteCategory(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	std::string category[20];
	int category_num;
	std::string category_name;
	char cate_name[20];
	int reset_date = 0;

	switch (iMsg)
	{
	case WM_INITDIALOG:
		category_num = __Get_all_file(category, MASTER);
		cate_combo = GetDlgItem(hDlg, IDC_CATEGORY);
		for (int i = 0; i < category_num; i++)
		{
			for (int j = 0; j < 4; j++)
				category[i].pop_back();
			SendMessage(cate_combo, CB_ADDSTRING, 0, (LPARAM)category[i].c_str());
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDADD:
			GetDlgItemText(hDlg, IDC_CATEGORY, cate_name, 20);
			reset_date = GetDlgItemInt(hDlg, IDC_RESET_DATE, NULL, FALSE);

			category_name = cate_name;

			if (category_name == "")
			{
				MessageBox(hDlg, "카테고리 이름을 입력하세요.", "ERROR", MB_OK);
			}
			else if (reset_date < 1 || reset_date > 31)
			{
				MessageBox(hDlg, "사용할 수 없는 날짜입니다.", "ERROR", MB_OK);
			}
			else if (__File_exist("Master/" + category_name + ".txt") == 1)
			{
				MessageBox(hDlg, "이미 있는 카테고리입니다.", "ERROR", MB_OK);
			}
			else
			{
				__Create_category(category_name, reset_date);
				EndDialog(hDlg, 0);
			}
			break;
		case IDDELETE:
			GetDlgItemText(hDlg, IDC_CATEGORY, cate_name, 20);

			category_name = cate_name;

			if (__File_exist("Master/" + category_name + ".txt") == 0)
			{
				MessageBox(hDlg, "존재하지 않는 카테고리입니다.", "ERROR", MB_OK);
			}
			else if (category_name == "전체")
			{
				MessageBox(hDlg, "\"전체\" 카테고리는 지울 수 없습니다.", "ERROR", MB_OK);
			}
			else
			{
				__Remove_category(category_name);
				EndDialog(hDlg, 0);
			}
			break;
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