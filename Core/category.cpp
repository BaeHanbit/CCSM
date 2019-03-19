#include "category.h"
#include "day.h"
#include "function.h"

std::list<std::pair<Category*,std::string>>cate_ptrs;

Category::Category(const std::string& name, const std::string & reset_date)
	:_income(),_expense()
{
	_money = 0;
	_name = name;
	_reset_date = reset_date;
}
Category::~Category()
{

}
///-----------------------------------------------------------------------------------------------
const std::string Category::__get_next_reset_date()
{
	return _next_reset_date;
}

void Category::__set_next_reset_date(std::string temp_date)
{
	_next_reset_date = temp_date;
}

bool Category::__get_flag_reset()
{
	return _flag_reset;
}

void Category::__set_flag_reset(bool A)
{
	_flag_reset = A;
}

std::string Category::__get_reset_date()
{
	return _reset_date;
}

void Category::__set_reset_date(std::string day)
{
	_reset_date = day;
}

///-----------------------------------------------------------------------------------------------
/*
	[ Function Act ]
	add income to money member.
	record income
*/
void Category::__add_income(const int & money, const std::string & text, const std::string & time)
{
	int index = _income.size();
	_income[index] = std::make_pair(money, std::make_pair(text, time));
	_money += money;
}

/*
	[ Function Act ]
		record expense
		subtract expense to money member.
*/
void Category::__add_expense(const int &money, const std::string & text, const std::string & time)
{
	int index = _expense.size();
	_expense[index] = std::make_pair(money, std::make_pair(text, time));
	_money -= money;
}

/*
	[return value]
	string : available value

	[what function does]
	return available date.
*/
const std::string Category::__validate_date()
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	int mon = tm.tm_mon + 1;
	int year = tm.tm_year + 1900;
	BOOL flag_leap_year = 0;

	if (year % 400 == 0)
	{
		flag_leap_year = 1;
	}
	else if (year % 100 == 0)
	{
		flag_leap_year = 0;
	}
	else if (year % 4 == 0)
	{
		flag_leap_year = 1;
	}
	else
	{
		flag_leap_year = 0;
	}

	if (std::stoi(_reset_date) <= 28)
	{
		return _reset_date;
	}
	else
	{

		if (mon == 2 && flag_leap_year == 0)
		{
			return std::string("28");
		}
		else if (mon == 2 && flag_leap_year == 1)
		{
			return std::string("29");
		}
		
		if (mon == 2 || mon == 4 || mon == 6 || mon == 9 || mon == 11)
		{
			if (std::stoi(_reset_date) == 29)
			{
				return std::string("29");
			}
			else
			{
				return std::string("30");
			}
		}
		else
		{
			if (std::stoi(_reset_date) == 29)
			{
				return std::string("29");
			}
			else if (std::stoi(_reset_date) == 30)
			{
				return std::string("30");
			}
			else
			{
				return std::string("31");
			}
		}

		return std::string("-1");

	}//end of if
}

///-------------------------------------------------------------------------------------------------
/*
	[return value]
	Category pointer will be return which is matched with input category name.

	[what function does]
	find category pointer in list.
	return category pointer.
*/
const Category* __find_category(std::string name)
{
	for (std::list<std::pair<Category*, std::string>>::iterator iter = cate_ptrs.begin();
		iter != cate_ptrs.end(); iter++)
	{
		if (name == iter->second)
		{
			return iter->first;
		}
		else
		{
			continue;
		}
	}
	return NULL;
}

/*
	[return value]
	-1 : fail to create category cause there is already same name.
	0 : achieve creating.

	[what function does]
	get info of new category
	create value in cate_ptrs;
*/
const int __create_category()
{
	Category* c_cate_temp = NULL;
	std::string name, date;
	{
		std::cout << "[Create Category]" << std::endl;
		std::cout << "Plz input Category name : ";
		std::cin >> name;
		std::cout << "Plz input reset date of Category : ";
		std::cin >> date;
	}
	if (__check_category_overlap(name))
	{
		std::cout << "[same category name has already existed!]" << std::endl;
		return -1;
	}

	c_cate_temp = new Category(name, date);
	cate_ptrs.push_back(std::pair<Category*,std::string>(c_cate_temp, name));

	return 0;
}

/*
	[return value]
	-1 : Fail to remove cause there isn't name.
	0 : achieve removing

	[what function does]
	get info of category which will be deleted
	remove value in cate_ptrs;
*/
const int __remove_category()
{
	std::string name;
	
	{
		std::cout << "[Remove Category]" << std::endl;
		std::cout << "Please input Category name : ";
		std::cin >> name;
	}

	if (__check_category_overlap(name))
	{
		for (std::list<std::pair<Category*, std::string>>::iterator iter = cate_ptrs.begin();
			iter != cate_ptrs.end(); iter++)
		{
			if (iter->second == name)
			{
				std::cout << "Found Same name" << std::endl;
				delete iter->first;
				cate_ptrs.erase(iter);
				return 0;
			}
		}
	}
	else
	{
		std::cout << "No Category has founded" << std::endl;
		return -1;
	}
}

/*
	[return value]
	0 : Failed to find
	1 : achieve finding


	[what function does]
	return whether he find same category_name
*/
const int __check_category_overlap(std::string name)
{
	for (std::list<std::pair<Category*, std::string>>::iterator iter = cate_ptrs.begin();
		iter != cate_ptrs.end(); iter++)
	{
		if (iter->second == name)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	return 0;
}

/*
	[ Return Value ]
	{0} : Do not need to reset
	{1} : When it need to reset

	[ Function Act ]
	Check that category need to be reseted.
*/
const int __check_category_reset(Category* temp)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	if (temp->__get_next_reset_date() == __today_date() && temp->__get_flag_reset() == true)
	{
		return 1;
	}
	else
		return 0;
}

/*
	[ Return Value ]
	{0} : Normal
	{-1} : Error : flag is false.

	[ Function Act ]
	set new _next_reset_date in category
*/
const int __reset_cate_date(Category* temp)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	std::string reset_date = temp->__get_reset_date();
	std::string next_reset_date = temp->__get_next_reset_date();
	bool flag = temp->__get_flag_reset();

	int year, mon, day;
	if (flag == true)
	{
		if (tm.tm_mon == 11)
		{
			year = tm.tm_year + 1901;
			mon = 1;
		}
		else
		{
			year = tm.tm_year + 1900;
			mon = tm.tm_mon + 2;
		}

		if (std::stoi(temp->__get_reset_date()) > __maximum_day(mon, year))
		{
			day = __maximum_day(mon, year);
		}
		else
		{
			day = std::stoi(temp->__get_reset_date());
		}

		std::string temp_date = __make_perfect_date(year, mon, day);

		temp->__set_next_reset_date(temp_date);
		temp->__set_flag_reset(false);
		return 0;
	}
	else
	{
		return -1;
	}
	
}

/*
	이후, DB에서 확인 할 것
	초기화 할때 0으로 만들고
	
	애초에 플레그 2개 있어야함
	초기화 할 필요가 있는지
	초기화 되었는지

	
*/
const int __remake_reset_flag(Category* temp)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	std::string reset_date = temp->__get_reset_date();
	int year, mon, day;
	year = tm.tm_year + 1900;
	mon = tm.tm_mon + 1;
	day = tm.tm_mday;

	std::string today = __make_perfect_date(year, mon, day);

	return -1;
}

const int __get_amount_of_category(Category* temp, std::string start_date, std::string end_date, int flag)
{

	if (flag == 1)
	{
		
	}
	else if (flag == 2)
	{

	}
	else if (flag == 3)
	{

	}
	else
	{
		return -1;
	}
}