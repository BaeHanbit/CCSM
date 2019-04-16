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
LRESULT CALLBACK DrawGraph_Day(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam, HDC hdc, RECT rectView);
LRESULT CALLBACK DrawGraph_Month(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam, HDC hdc, RECT rectView);
BOOL CALLBACK Dlg_AddDay(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Dlg_DayHistory(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Dlg_DeleteCategory(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
void DestroyButton();
void DrawLine(HDC hdc, RECT rectView);
void Get_income_expense_balance_day(int year, int month);
void Get_income_expense_balance_month(int year);
HINSTANCE hInst;
// 메인 = 0, 카테고리 = 1, 통계 = 2, < = 3, > = 4, Day = 5, edit = 6, dayAdd = 7, 카테고리 상세 기록 = 8
// 카테고리 콤보박스 = 9, 카테고리 추가 다이얼로그 = 10, 일별 월별 카테고리별 11 12 13, 지출 수입 잔액 14 15 16 

int status = 0, sub_status = 0, ieb = 0;
HWND hButton1, hButton2, h3, h4, ccsm, editButton, dayAdd, category_His, add_category;
HWND cate_combo;
HWND filter1[3];
HWND filter2[3], filter3[6];
HWND scroll, category_page;
HWND hwnd, dlg;
int TempPos, height = 0;
HFONT hFont, oldFont;

int selected_category;
extern std::map<std::string, weekday> calender;
std::vector<std::string> cate_history;
std::string temp_cate_history[8];
int cateHis_num;

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

int ieb_arr[3][31] = { 0, };
int ieb_arr_year[3][12] = { 0, };

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
	TCHAR boxNum[5];
	TCHAR category_percent[50];
	short zDelta;
	int category_income, category_expense, category_total_income = 0, category_total_expense = 0;

	switch (iMsg)
	{
	case WM_CREATE:
		__Create_dir(CATEGORY);
		__Create_dir(MASTER);
		__Create_index_file();
		__Set_calender();
		if (__File_exist(MASTER + "전체") == 0)
		{
			__Create_category("전체", 1);
		}
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
			cate_history.clear();
			selected_month = today_month_i;
			selected_year = today_year_i;
			InvalidateRgn(hwnd, NULL, TRUE);
			hButton1 = CreateWindow(TEXT("button"), TEXT("카테고리"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectView.right * 0.8, rectView.bottom * 0.0525, rectView.right * 0.2, rectView.bottom * 0.4, hwnd, (HMENU)1, hInst, NULL);
			hButton2 = CreateWindow(TEXT("button"), TEXT("메인 페이지"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectView.right * 0.8, rectView.bottom * 0.46 - 2, rectView.right * 0.2, rectView.bottom * 0.39, hwnd, (HMENU)0, hInst, NULL);
			h3 = CreateWindow(TEXT("button"), TEXT("<"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectView.right * 0.8, rectView.bottom * 0.85, rectView.right * 0.1, rectView.bottom * 0.15, hwnd, (HMENU)3, hInst, NULL);
			h4 = CreateWindow(TEXT("button"), TEXT(">"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectView.right * 0.9, rectView.bottom * 0.85, rectView.right * 0.1, rectView.bottom * 0.15, hwnd, (HMENU)4, hInst, NULL);
			filter1[0] = CreateWindow(TEXT("button"), TEXT("일별"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, rectView.right * 0.14, rectView.bottom * 0.075, 100, 30, hwnd, (HMENU)11, hInst, NULL);
			filter1[1] = CreateWindow(TEXT("button"), TEXT("월별"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, rectView.right * 0.23, rectView.bottom * 0.075, 100, 30, hwnd, (HMENU)12, hInst, NULL);
			filter1[2] = CreateWindow(TEXT("button"), TEXT("카테고리별"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, rectView.right * 0.32, rectView.bottom * 0.075, 100, 30, hwnd, (HMENU)13, hInst, NULL);
			
			CheckRadioButton(hwnd, 11, 13, 11 + sub_status);

			if (sub_status != 2)
			{
				filter2[0] = CreateWindow(TEXT("button"), TEXT("수입"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, rectView.right * 0.49, rectView.bottom * 0.075, 100, 30, hwnd, (HMENU)14, hInst, NULL);
				filter2[1] = CreateWindow(TEXT("button"), TEXT("지출"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, rectView.right * 0.6, rectView.bottom * 0.075, 100, 30, hwnd, (HMENU)15, hInst, NULL);
				filter2[2] = CreateWindow(TEXT("button"), TEXT("잔액"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, rectView.right * 0.71, rectView.bottom * 0.075, 100, 30, hwnd, (HMENU)16, hInst, NULL);
				CheckRadioButton(hwnd, 14, 16, 14 + ieb);
			}
			else
			{
				filter3[0] = CreateWindow(TEXT("combobox"), NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | ES_NUMBER, rectView.right * 0.5, rectView.bottom * 0.06, 70, 20, hwnd, (HMENU)17, hInst, NULL);
				filter3[1] = CreateWindow(TEXT("combobox"), NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | ES_NUMBER, rectView.right * 0.6, rectView.bottom * 0.06, 40, 20, hwnd, (HMENU)18, hInst, NULL);
				filter3[2] = CreateWindow(TEXT("combobox"), NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | ES_NUMBER, rectView.right * 0.67, rectView.bottom * 0.06, 40, 20, hwnd, (HMENU)19, hInst, NULL);
				filter3[3] = CreateWindow(TEXT("combobox"), NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | ES_NUMBER, rectView.right * 0.5, rectView.bottom * 0.1, 70, 20, hwnd, (HMENU)20, hInst, NULL);
				filter3[4] = CreateWindow(TEXT("combobox"), NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | ES_NUMBER, rectView.right * 0.6, rectView.bottom * 0.1, 40, 20, hwnd, (HMENU)21, hInst, NULL);
				filter3[5] = CreateWindow(TEXT("combobox"), NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | ES_NUMBER, rectView.right * 0.67, rectView.bottom * 0.1, 40, 20, hwnd, (HMENU)22, hInst, NULL);
			}
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
			case 2:
				switch (sub_status)
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
				case 1:
					selected_year -= 1;
					break;
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
				break;
			case 2:
				switch (sub_status)
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
					break;
				case 1:
					selected_year += 1;
					break;
				}
				break;
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
			cate_history.clear();
			cateHis_num = 0;

			scroll = CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | SBS_VERT, rectView.right * 0.98, rectView.bottom * 0.05, rectView.right * 0.02, rectView.bottom - rectView.bottom * 0.05, hwnd, NULL, hInst, NULL);
			cate_combo = CreateWindow(TEXT("combobox"), NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, rectView.right * 0.12, rectView.bottom * 0.91, rectView.right * 0.13, rectView.bottom * 0.2, hwnd, (HMENU)9, hInst, NULL);
			for (int i = 0; i < cate_num; i++)
			{
				SendMessage(cate_combo, CB_ADDSTRING, NULL, (LPARAM)category_name[i].c_str());
			}
			SendMessage(cate_combo, CB_SETCURSEL, selected_category, 0);
			int t;
			t = 0;
			while (__Get_data(temp_cate_history, category_name[selected_category], today_year_s + '-' + today_month_s, t * 8, 8) != -2)
			{
				for (int i = 0; i < 8; i++)
				{
					cate_history.push_back(temp_cate_history[i]);
					temp_cate_history[i] = "";
				}
				t++;
			}
			cateHis_num += t * 8;
			t = 0;
			while (temp_cate_history[t] != "")
			{
				cate_history.push_back(temp_cate_history[t]);
				t++;
			}
			cateHis_num += t;
			if (cateHis_num > 8)
			{
				SetScrollRange(scroll, SB_CTL, 0, boxHeight * (cateHis_num - 8), TRUE);
			}
			else
			{
				SetScrollRange(scroll, SB_CTL, 0, 0, TRUE);
			}
			SetScrollPos(scroll, SB_CTL, 0, TRUE);
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		case 9:
			switch (HIWORD(wParam)) {
			case CBN_SELCHANGE:
				selected_category = SendMessage(cate_combo, CB_GETCURSEL, 0, 0);
				SendMessage(hwnd, WM_COMMAND, 8, NULL);
				break;
			}
			break;
		case 10:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_ADD_DELETE_CATEGORY), hwnd, (DLGPROC)Dlg_DeleteCategory);
			SendMessage(hwnd, WM_COMMAND, 1, NULL);
			break;
		case 11:
			sub_status = 0;
			SendMessage(hwnd, WM_COMMAND, 2, 0);
			break;
		case 12:
			sub_status = 1;
			SendMessage(hwnd, WM_COMMAND, 2, 0);
			break;
		case 13:
			sub_status = 2;
			SendMessage(hwnd, WM_COMMAND, 2, 0);
			break;
		case 14:
			ieb = 0;
			SendMessage(hwnd, WM_COMMAND, 2, 0);
			break;
		case 15:
			ieb = 1;
			SendMessage(hwnd, WM_COMMAND, 2, 0);
			break;
		case 16:
			ieb = 2;
			SendMessage(hwnd, WM_COMMAND, 2, 0);
			break;
		}
		break;
	case WM_CTLCOLORSTATIC:
		return (LRESULT)CreateSolidBrush(RGB(255, 255, 255));
		break;
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
			selected_month_day = __Maximum_day(selected_month, selected_year);


			DrawLine(hdc, rectView);
			Rectangle(hdc, 0, rectView.bottom * 0.05, rectView.right * 0.1, rectView.bottom * 0.14);
			Rectangle(hdc, rectView.right * 0.1, rectView.bottom * 0.05, rectView.right * 0.45, rectView.bottom * 0.14);
			Rectangle(hdc, rectView.right * 0.45, rectView.bottom * 0.05, rectView.right * 0.8, rectView.bottom * 0.14);
			Rectangle(hdc, 0, rectView.bottom * 0.05, rectView.right * 0.1, rectView.bottom * 0.14);
			TextOut(hdc, rectView.right * 0.035, rectView.bottom * 0.085, "필터", _tcslen("필터"));


			switch (sub_status)
			{
			case 0:
				Get_income_expense_balance_day(selected_year, selected_month);
				DrawGraph_Day(hwnd, iMsg, wParam, lParam, hdc, rectView);
				break;
			case 1:
				Get_income_expense_balance_month(selected_year);
				DrawGraph_Month(hwnd, iMsg, wParam, lParam, hdc, rectView);
				break;
			case 2:
				TextOut(hdc, rectView.right * 0.56, rectView.bottom * 0.065, "년", _tcslen("년"));
				TextOut(hdc, rectView.right * 0.64, rectView.bottom * 0.065, "월", _tcslen("월"));
				TextOut(hdc, rectView.right * 0.71, rectView.bottom * 0.065, "일 부터", _tcslen("일 부터"));
				TextOut(hdc, rectView.right * 0.56, rectView.bottom * 0.105, "년", _tcslen("년"));
				TextOut(hdc, rectView.right * 0.64, rectView.bottom * 0.105, "월", _tcslen("월"));
				TextOut(hdc, rectView.right * 0.71, rectView.bottom * 0.105, "일 까지", _tcslen("일 까지"));
				break;
			}
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
			std::string cate_history_info[4];
			std::string cate_history_date[5];
			std::string cate_history_date_complete;
			char str_buff[100];
			char *tok;
			int str_cnt;
			int flag;
			TCHAR total_m[50];
			std::string::size_type n;
			int total_money;
			total_money = 0;

			total_money = __Get_total_income(category_name[selected_category], today_year_s + '-' + today_month_s) - __Get_total_expense(category_name[selected_category], today_year_s + '-' + today_month_s);

			for (int i = 1; i <= cateHis_num; i++)
			{
				strcpy(str_buff, cate_history[i - 1].c_str());
				tok = strtok(str_buff, "|");
				str_cnt = 0;
				while (tok != nullptr) {
					cate_history_info[str_cnt++] = std::string(tok);
					tok = strtok(nullptr, "|");
				}

				strcpy(str_buff, cate_history_info[1].c_str());
				tok = strtok(str_buff, "-");
				str_cnt = 0;
				while (tok != nullptr) {
					cate_history_date[str_cnt++] = std::string(tok);
					tok = strtok(nullptr, "-");
				}

				cate_history_info[2].insert(0, "[");
				cate_history_info[2].append("]");

				cate_history_date_complete = "<" + cate_history_date[0] + " " + cate_history_date[1] + "-" +
					cate_history_date[2] + " " + cate_history_date[3] + ":" + cate_history_date[4] + ">";

				n = cate_history_info[3].find("+");

				cate_history_info[3].erase(0, 1);

				cate_history_info[3].insert(0, "\\ ");

				if (i == 1)
				{
					Rectangle(hdc, 0, rectView.bottom * 0.05 - height, rectView.right *  0.98, rectView.bottom * 0.05 + boxHeight - height);
					SetBkMode(hdc, TRANSPARENT);
					TextOut(hdc, rectView.right * 0.12, rectView.bottom * 0.04 + boxHeight / 2 - height, cate_history_date_complete.c_str(), _tcslen(cate_history_date_complete.c_str()));
					TextOut(hdc, rectView.right * 0.35, rectView.bottom * 0.04 + boxHeight / 2 - height, cate_history_info[3].c_str(), _tcslen(cate_history_info[3].c_str()));
					if (n == std::string::npos)
						TextOut(hdc, rectView.right * 0.55, rectView.bottom * 0.04 + boxHeight / 2 - height, "[지출]", _tcslen("[지출]"));
					else
						TextOut(hdc, rectView.right * 0.55, rectView.bottom * 0.04 + boxHeight / 2 - height, "[수입]", _tcslen("[수입]"));

					TextOut(hdc, rectView.right * 0.75, rectView.bottom * 0.04 + boxHeight / 2 - height, cate_history_info[2].c_str(), _tcslen(cate_history_info[2].c_str()));
				}
				else
				{
					Rectangle(hdc, 0, rectView.bottom * 0.05 + boxHeight * (i - 1) - height, rectView.right *  0.98, rectView.bottom * 0.05 + boxHeight * i - height);
					SetBkMode(hdc, TRANSPARENT);
					TextOut(hdc, rectView.right * 0.12, rectView.bottom * 0.04 + boxHeight * (i - 1) + boxHeight / 2 - height, cate_history_date_complete.c_str(), _tcslen(cate_history_date_complete.c_str()));
					TextOut(hdc, rectView.right * 0.35, rectView.bottom * 0.04 + boxHeight * (i - 1) + boxHeight / 2 - height, cate_history_info[3].c_str(), _tcslen(cate_history_info[3].c_str()));
					if (n == std::string::npos)
						TextOut(hdc, rectView.right * 0.55, rectView.bottom * 0.04 + boxHeight * (i - 1) + boxHeight / 2 - height, "[지출]", _tcslen("[지출]"));
					else
						TextOut(hdc, rectView.right * 0.55, rectView.bottom * 0.04 + boxHeight * (i - 1) + boxHeight / 2 - height, "[수입]", _tcslen("[수입]"));
					TextOut(hdc, rectView.right * 0.75, rectView.bottom * 0.04 + boxHeight * (i - 1) + boxHeight / 2 - height, cate_history_info[2].c_str(), _tcslen(cate_history_info[2].c_str()));
				}
			}

			wsprintf(total_m, "전체 잔액 : %d", total_money);
			hBrush = CreateSolidBrush(RGB(133, 133, 133));
			SelectObject(hdc, hBrush);

			SetBkMode(hdc, TRANSPARENT);
			Rectangle(hdc, 0, rectView.bottom * 0.85, rectView.right *  0.98, rectView.bottom);
			TextOut(hdc, rectView.right * 0.40, rectView.bottom * 0.85 + rectView.bottom * 0.07, total_m, _tcslen(total_m));
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
			if (cateHis_num <= 8)
				return 1;
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
			if (cateHis_num <= 8)
				return 1;

			zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

			TempPos = height;

			if (zDelta < 0)
				TempPos = min(boxHeight * (cateHis_num - 8), height + boxHeight);
			else
			{
				TempPos = max(0, height - boxHeight);
			}
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
	for (int i = 0; i < 3; i++)
	{
		DestroyWindow(filter1[i]);
		DestroyWindow(filter2[i]);
		DestroyWindow(filter3[i]);
		DestroyWindow(filter3[i + 3]);
	}
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
	std::string day_s, month_s;
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
			if (month < 10)
			{
				month_s = '0' + std::to_string(month);
			}
			else
			{
				month_s = std::to_string(month);
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
			else
			{
				day_s = std::to_string(day);
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
			else
			{
				hour_s = std::to_string(hour);
			}
			
			if (minute < 10)
			{
				minute_s = '0' + std::to_string(minute);
			}
			else
			{
				minute_s = std::to_string(minute);
			}

			money = GetDlgItemInt(hDlg, IDC_EDIT_MONEY, NULL, TRUE);
			if (money < 1)
			{
				MessageBox(hDlg, "올바른 금액을 입력하세요", "Error", MB_OK);
				break;
			}
			GetDlgItemText(hDlg, IDC_EDIT_MEMO, memo, 40);

			__Insert_cate_data(c_name, std::to_string(year) + '-' + month_s + '-' + day_s + '-' + hour_s + '-' + minute_s, memo, money, ie);
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
	int category_num = 0;
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
			if (category_num == 20)
			{
				MessageBox(hDlg, "카테고리 최대 개수는 20개입니다.", "Error", MB_OK);
				break;
			}
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

LRESULT CALLBACK DrawGraph_Day(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam, HDC hdc, RECT rectView)
{
	int i_max = 0, e_max = 0, b_max = 0, max;
	int flag;
	HPEN grayPen, oldPen, noPen;
	HBRUSH hBrush, oldBrush;
	double percent;
	std::string month;

	for (int i = 0; i < selected_month_day; i++)
	{
		if (ieb_arr[0][i] > i_max)
		{
			i_max = ieb_arr[0][i];
		}
		
		if (ieb_arr[1][i] > e_max)
		{
			e_max = ieb_arr[1][i];
		}

		if (ieb_arr[2][i] > b_max)
		{
			b_max = ieb_arr[2][i];
		}
	}

	hFont = CreateFont(11, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, "바탕체");
	oldFont = (HFONT)SelectObject(hdc, hFont);
	hBrush = CreateSolidBrush(RGB(151, 210, 229));
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	grayPen = CreatePen(PS_SOLID, 0, RGB(190, 190, 190));
	oldPen = (HPEN)SelectObject(hdc, grayPen);
	noPen = (HPEN)GetStockObject(NULL_PEN);

	switch (ieb)
	{
	case 0:
		flag = i_max / 100000 + 1;
		max = flag * 100000;

		hFont = CreateFont(15, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, "바탕체");
		SelectObject(hdc, hFont);

		month = std::to_string(selected_month) + "월";

		SetTextAlign(hdc, TA_LEFT);
		TextOut(hdc, rectView.right * 0.4, rectView.bottom * 0.17, month.c_str(), _tcslen(month.c_str()));

		SetTextAlign(hdc, TA_RIGHT);
		hFont = CreateFont(11, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, "바탕체");
		SelectObject(hdc, hFont);

		for (int i = 0; i <= 10; i++)
		{
			TextOut(hdc, rectView.right * 0.1, rectView.bottom * 0.88 - (rectView.bottom * 0.065) * i, std::to_string(i * flag * 10000).c_str(), _tcslen(std::to_string(i * flag * 10000).c_str()));
		}
		
		SelectObject(hdc, grayPen);

		for (int i = 0; i <= 10; i++)
		{
			MoveToEx(hdc, rectView.right * 0.12, rectView.bottom * 0.89 - (rectView.bottom * 0.065) * i, NULL);
			LineTo(hdc, rectView.right * 0.74, rectView.bottom * 0.89 - (rectView.bottom * 0.065) * i);
		}

		SelectObject(hdc, noPen);

		SetTextAlign(hdc, TA_LEFT);

		for (int i = 1; i <= selected_month_day; i++)
		{
			TextOut(hdc, rectView.right * 0.105 + rectView.right * 0.02 * i, rectView.bottom * 0.9, std::to_string(i).c_str(), _tcslen(std::to_string(i).c_str()));
		}

		for (int i = 1; i <= selected_month_day; i++)
		{
			if (ieb_arr[0][i - 1] != 0)
			{
				percent = 1 - (double)ieb_arr[0][i - 1] / max;
				Rectangle(hdc, rectView.right * 0.105 + rectView.right * 0.02 * i, (rectView.bottom * 0.24) + ((rectView.bottom * 0.65) * percent), rectView.right * 0.105 + rectView.right * 0.02 * i + rectView.right * 0.01, rectView.bottom * 0.89);
			}
		}
		break;
	case 1:
		flag = e_max / 100000 + 1;
		max = flag * 100000;

		hFont = CreateFont(15, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, "바탕체");
		SelectObject(hdc, hFont);

		month = std::to_string(selected_month) + "월";

		SetTextAlign(hdc, TA_LEFT);
		TextOut(hdc, rectView.right * 0.4, rectView.bottom * 0.17, month.c_str(), _tcslen(month.c_str()));

		SetTextAlign(hdc, TA_RIGHT);
		hFont = CreateFont(11, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, "바탕체");
		SelectObject(hdc, hFont);

		for (int i = 0; i <= 10; i++)
		{
			TextOut(hdc, rectView.right * 0.1, rectView.bottom * 0.88 - (rectView.bottom * 0.065) * i, std::to_string(i * flag * 10000).c_str(), _tcslen(std::to_string(i * flag * 10000).c_str()));
		}

		SelectObject(hdc, grayPen);

		for (int i = 0; i <= 10; i++)
		{
			MoveToEx(hdc, rectView.right * 0.12, rectView.bottom * 0.89 - (rectView.bottom * 0.065) * i, NULL);
			LineTo(hdc, rectView.right * 0.74, rectView.bottom * 0.89 - (rectView.bottom * 0.065) * i);
		}
		SelectObject(hdc, noPen);

		SetTextAlign(hdc, TA_LEFT);

		for (int i = 1; i <= selected_month_day; i++)
		{
			TextOut(hdc, rectView.right * 0.105 + rectView.right * 0.02 * i, rectView.bottom * 0.9, std::to_string(i).c_str(), _tcslen(std::to_string(i).c_str()));
		}

		for (int i = 1; i <= selected_month_day; i++)
		{
			if (ieb_arr[1][i - 1] != 0)
			{
				percent = 1 - (double)ieb_arr[1][i - 1] / max;
				Rectangle(hdc, rectView.right * 0.105 + rectView.right * 0.02 * i, (rectView.bottom * 0.24) + ((rectView.bottom * 0.65) * percent), rectView.right * 0.105 + rectView.right * 0.02 * i + rectView.right * 0.01, rectView.bottom * 0.89);
			}
		}
		break;
	case 2:
		flag = b_max / 100000 + 1;
		max = flag * 100000;

		hFont = CreateFont(15, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, "바탕체");
		SelectObject(hdc, hFont);

		month = std::to_string(selected_month) + "월";

		SetTextAlign(hdc, TA_LEFT);
		TextOut(hdc, rectView.right * 0.4, rectView.bottom * 0.17, month.c_str(), _tcslen(month.c_str()));

		SetTextAlign(hdc, TA_RIGHT);
		hFont = CreateFont(11, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, "바탕체");
		SelectObject(hdc, hFont);

		for (int i = 0; i <= 10; i++)
		{
			TextOut(hdc, rectView.right * 0.1, rectView.bottom * 0.88 - (rectView.bottom * 0.065) * i, std::to_string(i * flag * 10000).c_str(), _tcslen(std::to_string(i * flag * 10000).c_str()));
		}

		SelectObject(hdc, grayPen);

		for (int i = 0; i <= 10; i++)
		{
			MoveToEx(hdc, rectView.right * 0.12, rectView.bottom * 0.89 - (rectView.bottom * 0.065) * i, NULL);
			LineTo(hdc, rectView.right * 0.74, rectView.bottom * 0.89 - (rectView.bottom * 0.065) * i);
		}
		SelectObject(hdc, noPen);

		SetTextAlign(hdc, TA_LEFT);

		for (int i = 1; i <= selected_month_day; i++)
		{
			TextOut(hdc, rectView.right * 0.105 + rectView.right * 0.02 * i, rectView.bottom * 0.9, std::to_string(i).c_str(), _tcslen(std::to_string(i).c_str()));
		}

		for (int i = 1; i <= selected_month_day; i++)
		{
			if (ieb_arr[2][i - 1] != 0)
			{
				percent = 1 - (double)ieb_arr[2][i - 1] / max;
				Rectangle(hdc, rectView.right * 0.105 + rectView.right * 0.02 * i, (rectView.bottom * 0.24) + ((rectView.bottom * 0.65) * percent), rectView.right * 0.105 + rectView.right * 0.02 * i + rectView.right * 0.01, rectView.bottom * 0.89);
			}
		}
		break;
	}

	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
	SelectObject(hdc, oldPen);
	DeleteObject(grayPen);
	DeleteObject(noPen);

	return 0;
}

LRESULT CALLBACK DrawGraph_Month(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam, HDC hdc, RECT rectView)
{
	int i_max = 0, e_max = 0, b_max = 0, max;
	int flag;
	HPEN grayPen, oldPen, noPen;
	HBRUSH hBrush, oldBrush;
	double percent;
	std::string year;

	for (int i = 0; i < 12; i++)
	{
		if (ieb_arr_year[0][i] > i_max)
		{
			i_max = ieb_arr_year[0][i];
		}

		if (ieb_arr_year[1][i] > e_max)
		{
			e_max = ieb_arr_year[1][i];
		}

		if (ieb_arr_year[2][i] > b_max)
		{
			b_max = ieb_arr_year[2][i];
		}
	}

	hFont = CreateFont(11, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, "바탕체");
	oldFont = (HFONT)SelectObject(hdc, hFont);
	hBrush = CreateSolidBrush(RGB(151, 210, 229));
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	grayPen = CreatePen(PS_SOLID, 0, RGB(190, 190, 190));
	oldPen = (HPEN)SelectObject(hdc, grayPen);
	noPen = (HPEN)GetStockObject(NULL_PEN);

	switch (ieb)
	{
	case 0:
		flag = i_max / 1000000 + 1;
		max = flag * 1000000;

		hFont = CreateFont(15, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, "바탕체");
		SelectObject(hdc, hFont);

		year = std::to_string(selected_year) + "년";

		SetTextAlign(hdc, TA_LEFT);
		TextOut(hdc, rectView.right * 0.4, rectView.bottom * 0.17, year.c_str(), _tcslen(year.c_str()));

		SetTextAlign(hdc, TA_RIGHT);
		hFont = CreateFont(11, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, "바탕체");
		SelectObject(hdc, hFont);

		for (int i = 0; i <= 10; i++)
		{
			TextOut(hdc, rectView.right * 0.1, rectView.bottom * 0.88 - (rectView.bottom * 0.065) * i, std::to_string(i * flag * 100000).c_str(), _tcslen(std::to_string(i * flag * 100000).c_str()));
		}

		SelectObject(hdc, grayPen);

		for (int i = 0; i <= 10; i++)
		{
			MoveToEx(hdc, rectView.right * 0.12, rectView.bottom * 0.89 - (rectView.bottom * 0.065) * i, NULL);
			LineTo(hdc, rectView.right * 0.74, rectView.bottom * 0.89 - (rectView.bottom * 0.065) * i);
		}

		SelectObject(hdc, noPen);

		SetTextAlign(hdc, TA_LEFT);

		for (int i = 1; i <= 12; i++)
		{
			TextOut(hdc, rectView.right * 0.105 + rectView.right * 0.05 * i, rectView.bottom * 0.9, std::to_string(i).c_str(), _tcslen(std::to_string(i).c_str()));
		}

		for (int i = 1; i <= 12; i++)
		{
			if (ieb_arr_year[0][i - 1] != 0)
			{
				percent = 1 - (double)ieb_arr_year[0][i - 1] / max;
				Rectangle(hdc, rectView.right * 0.094 + rectView.right * 0.05 * i, (rectView.bottom * 0.24) + ((rectView.bottom * 0.65) * percent), rectView.right * 0.094 + rectView.right * 0.05 * i + rectView.right * 0.03, rectView.bottom * 0.89);
			}
		}
		break;
	case 1:
		flag = e_max / 1000000 + 1;
		max = flag * 1000000;

		hFont = CreateFont(15, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, "바탕체");
		SelectObject(hdc, hFont);

		year = std::to_string(selected_year) + "년";

		SetTextAlign(hdc, TA_LEFT);
		TextOut(hdc, rectView.right * 0.4, rectView.bottom * 0.17, year.c_str(), _tcslen(year.c_str()));

		SetTextAlign(hdc, TA_RIGHT);
		hFont = CreateFont(11, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, "바탕체");
		SelectObject(hdc, hFont);

		for (int i = 0; i <= 10; i++)
		{
			TextOut(hdc, rectView.right * 0.1, rectView.bottom * 0.88 - (rectView.bottom * 0.065) * i, std::to_string(i * flag * 100000).c_str(), _tcslen(std::to_string(i * flag * 100000).c_str()));
		}

		SelectObject(hdc, grayPen);

		for (int i = 0; i <= 10; i++)
		{
			MoveToEx(hdc, rectView.right * 0.12, rectView.bottom * 0.89 - (rectView.bottom * 0.065) * i, NULL);
			LineTo(hdc, rectView.right * 0.74, rectView.bottom * 0.89 - (rectView.bottom * 0.065) * i);
		}

		SelectObject(hdc, noPen);

		SetTextAlign(hdc, TA_LEFT);

		for (int i = 1; i <= 12; i++)
		{
			TextOut(hdc, rectView.right * 0.105 + rectView.right * 0.05 * i, rectView.bottom * 0.9, std::to_string(i).c_str(), _tcslen(std::to_string(i).c_str()));
		}

		for (int i = 1; i <= 12; i++)
		{
			if (ieb_arr_year[1][i - 1] != 0)
			{
				percent = 1 - (double)ieb_arr_year[1][i - 1] / max;
				Rectangle(hdc, rectView.right * 0.094 + rectView.right * 0.05 * i, (rectView.bottom * 0.24) + ((rectView.bottom * 0.65) * percent), rectView.right * 0.094 + rectView.right * 0.05 * i + rectView.right * 0.03, rectView.bottom * 0.89);
			}
		}
		break;
	case 2:
		flag = b_max / 1000000 + 1;
		max = flag * 1000000;

		hFont = CreateFont(15, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, "바탕체");
		SelectObject(hdc, hFont);

		year = std::to_string(selected_year) + "년";

		SetTextAlign(hdc, TA_LEFT);
		TextOut(hdc, rectView.right * 0.4, rectView.bottom * 0.17, year.c_str(), _tcslen(year.c_str()));

		SetTextAlign(hdc, TA_RIGHT);
		hFont = CreateFont(11, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, "바탕체");
		SelectObject(hdc, hFont);

		for (int i = 0; i <= 10; i++)
		{
			TextOut(hdc, rectView.right * 0.1, rectView.bottom * 0.88 - (rectView.bottom * 0.065) * i, std::to_string(i * flag * 100000).c_str(), _tcslen(std::to_string(i * flag * 100000).c_str()));
		}

		SelectObject(hdc, grayPen);

		for (int i = 0; i <= 10; i++)
		{
			MoveToEx(hdc, rectView.right * 0.12, rectView.bottom * 0.89 - (rectView.bottom * 0.065) * i, NULL);
			LineTo(hdc, rectView.right * 0.74, rectView.bottom * 0.89 - (rectView.bottom * 0.065) * i);
		}

		SelectObject(hdc, noPen);

		SetTextAlign(hdc, TA_LEFT);

		for (int i = 1; i <= 12; i++)
		{
			TextOut(hdc, rectView.right * 0.105 + rectView.right * 0.05 * i, rectView.bottom * 0.9, std::to_string(i).c_str(), _tcslen(std::to_string(i).c_str()));
		}

		for (int i = 1; i <= 12; i++)
		{
			if (ieb_arr_year[2][i - 1] != 0)
			{
				percent = 1 - (double)ieb_arr_year[2][i - 1] / max;
				Rectangle(hdc, rectView.right * 0.094 + rectView.right * 0.05 * i, (rectView.bottom * 0.24) + ((rectView.bottom * 0.65) * percent), rectView.right * 0.094 + rectView.right * 0.05 * i + rectView.right * 0.03, rectView.bottom * 0.89);
			}
		}
		break;
	}
	return 0;
}

void Get_income_expense_balance_day(int year, int month)
{
	std::string year_s, month_s;
	std::string info[4], date[5];
	int t;
	char str_buff[100];
	char *tok;
	int str_cnt;
	std::string::size_type n;
	std::string location;

	year_s = std::to_string(year);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 31; j++)
		{
			ieb_arr[i][j] = 0;
		}
	}

	if (month < 10)
	{
		month_s = '0' + std::to_string(month);
	}
	else
	{
		month_s = std::to_string(month);
	}

	for (int i = 0; i < cate_num; i++)
	{
		cate_history.clear();
		cateHis_num = 0;
		t = 0;
		location = CATEGORY + category_name[i] + '/' + year_s + '-' + month_s + ".txt";
		if (__File_exist(location) == 0)
			continue;

		while (__Get_data(temp_cate_history, category_name[i], year_s + '-' + month_s, t * 8, 8) != -2)
		{
			for (int i = 0; i < 8; i++)
			{
				cate_history.push_back(temp_cate_history[i]);
				temp_cate_history[i] = "";
			}
			t++;
		}
		cateHis_num += t * 8;
		t = 0;
		while (temp_cate_history[t] != "")
		{
			cate_history.push_back(temp_cate_history[t]);
			t++;
		}
		cateHis_num += t;

		for (int j = 0; j < cateHis_num; j++)
		{
			strcpy(str_buff, cate_history[j].c_str());
			tok = strtok(str_buff, "|");
			str_cnt = 0;
			while (tok != nullptr) {
				info[str_cnt++] = std::string(tok);
				tok = strtok(nullptr, "|");
			}

			strcpy(str_buff, info[1].c_str());
			tok = strtok(str_buff, "-");
			str_cnt = 0;
			while (tok != nullptr) {
				date[str_cnt++] = std::string(tok);
				tok = strtok(nullptr, "-");
			}

			n = info[3].find("+");

			info[3].erase(0, 1);

			if (n == std::string::npos)
			{
				ieb_arr[1][std::stoi(date[2]) - 1] += std::stoi(info[3]);
				ieb_arr[2][std::stoi(date[2]) - 1] -= std::stoi(info[3]);
			}
			else
			{
				ieb_arr[0][std::stoi(date[2]) - 1] += std::stoi(info[3]);
				ieb_arr[2][std::stoi(date[2]) - 1] += std::stoi(info[3]);
			}
		}
	}
}

void Get_income_expense_balance_month(int year)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			ieb_arr_year[i][j] = 0;
		}
	}

	for (int i = 1; i <= 12; i++)
	{
		Get_income_expense_balance_day(year, i);

		for (int j = 0; j < selected_month_day; j++)
		{
			ieb_arr_year[0][i - 1] += ieb_arr[0][j];
			ieb_arr_year[1][i - 1] += ieb_arr[1][j];
			ieb_arr_year[2][i - 1] += ieb_arr[2][j];
		}
	}
}