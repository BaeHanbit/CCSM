#pragma once
#pragma warning(disable:4996)
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
#include <string>
#include <iterator>
#include <time.h>
#include <map>

template <typename T=int>
class Day
{
	enum check_type{INCOME=1,EXPENSE};
public:
	Day() :_income(0), _expense(0), _date() { calculate_date(); }
	~Day() {}

public:
	T Get_income() { return _income; }
	T Get_expense() { return _expense; }
	std::string Get_date() { return _date; }

public:
	void Set_income(T income) { _income = income; }
	void Set_expense(T expense) { _expense = expense; }
	void Set_date(std::string date) { _date = date; }

private:
	void calculate_date()
	{
		time_t date = time(NULL);
		struct tm st_tm = *localtime(&date);

		_date += std::to_string(st_tm.tm_year + 1900) + '-';
		_date += std::to_string(st_tm.tm_mon + 1) + '-';
		_date += std::to_string(st_tm.tm_mday);
	}
	const int expense_money(const int& cost, const std::string& category,const std::string& time, const std::string& memo)
	{

	}
	const int income_money(const int& cost,const std::string& category, const std::string&time, const std::string& memo)
	{
		_income += cost;//카테고리 무관한 당일 전체 수입 추가
		_history.insert(_history_index, INCOME, cost, memo);//기록 추가
		_history_time.insert(time);//기록 시간 추가

	}
private:
	static T _money;
	int _history_index;
	T _income;
	T _expense;
	std::string _date;
	std::map<int, std::pair<int, std::pair<int, std::string>>> _history;
	std::map<int, std::string>_history_time;
	//1. index, 2.지출 수입, 3.금액 4.팽이버섯 1=수입, 2= 지출
};