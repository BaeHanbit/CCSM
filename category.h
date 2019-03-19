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

const Category* __find_category(std::string name);
const int __create_category();
const int __remove_category();
const int __check_category_overlap(std::string name);
const int __check_category_reset(Category* temp);
const int __reset_cate_date(Category* temp);
const int __remake_reset_flag(Category* temp);

class Category
{
public:
	Category(const std::string& name, const std::string & reset_date);
	~Category();

public:
	const std::string __get_next_reset_date();
	void __set_next_reset_date(std::string temp_date);

	bool __get_flag_reset();
	void __set_flag_reset(bool A);

	std::string __get_reset_date();
	void __set_reset_date(std::string day);

public:
	const int __income(const int & money, const std::string & text, const std::string & time);
	const int __expense(const int &money, const std::string & text, const std::string & time);
	const std::string __validate_date();
	const int __get_total_income();
	const int __get_total_expense();


private:
	int _money;
	std::string _name;//must not be overlaped

	bool _flag_reset;//1 need to reset
	std::string _reset_date;
	std::string _next_reset_date;


	int _income_index;
	std::map<int,int> _income;//해당 카테고리 수입
	std::map<int,std::string>_income_text;
	std::map<int,std::string>_income_time;
	
	int _expense_index;
	std::map<int,int>_expense;//카테고리 지출
	std::map<int,std::string> _expense_text;
	std::map<int,std::string>_expense_time;
};