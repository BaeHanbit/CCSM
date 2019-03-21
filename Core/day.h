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

class Day;

const int __create_day(int year, int mon, int day);
const

class Day
{
	
public:
	Day(std::string date);
	~Day();


private:
	const std::string __get_today();
	const int __change_money(const std::string category_name, const int cost, const std::string time, const std::string text, bool type);

public:
	int _total_money;
	int _total_income;
	int _total_expense;

	int _max_sereial_number;
	std::string _date;

	std::map<int, std::string>_history_money;
	std::map<int, std::string>_history_time;
	std::map<int, std::string>_history_text;
	std::map<int, bool>_history_type;//0 : income, 1 : expense

};