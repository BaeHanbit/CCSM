#pragma once
#pragma warning(disable:4996)
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <time.h>
#include <conio.h>
#include <Windows.h>

class Category;

Category* __find_category(std::string name);
const int __create_category();
const int __remove_category();
int __fix_category(Category* temp, std::string name, std::string reset_date);
void __remove_category_record(Category* temp, int index, bool type);
const int __check_category_overlap(std::string name);
const int __check_category_reset(Category* temp);
const int __reset_cate_date(Category* temp);
const int __remake_reset_flag(Category* temp);
const int __get_amount_of_category(Category* temp, std::string start_date, std::string end_date, int flag);


class Category
{
public:
	Category(const std::string& name, const std::string & reset_date);
	~Category();

public:
	void __add_income(const int & money, const std::string & text, const std::string & time);
	void __add_expense(const int &money, const std::string & text, const std::string & time);
	const std::string __validate_date();

public:
	int _money;
	std::string _name;

	bool _flag_reset;
	std::string _reset_date;
	std::string _next_reset_date;

	int _max_income_serial_number;
	std::map<int, int>_income_money;
	std::map<int, std::string> _income_text;
	std::map<int, std::string>_income_time;
	
	int _max_expense_serial_number;
	std::map<int, int>_expense_money;
	std::map<int, std::string>_expense_text;
	std::map<int, std::string>_expense_time;
};