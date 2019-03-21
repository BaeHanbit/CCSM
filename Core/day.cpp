#include "day.h"
#include "category.h"
#include "function.h"

std::vector<Day*> day_ptrs;
enum {INCOME,EXPENSE};

Day::Day(std::string date)
	:_history_money(),_history_text(),_history_time()
{
	_date = date;
	_total_money = 0;
	_total_expense = 0;
	_total_income = 0;
	_max_sereial_number = 0;
}

Day::~Day()
{

}
///-------------------------------------------------------------------------------------------

const std::string Day::__get_today()
{
	time_t date = time(NULL);
	struct tm tm = *localtime(&date);

	int year, mon, day;
	year = tm.tm_year + 1900;
	mon = tm.tm_mon + 1;
	day = tm.tm_mday;

	return __make_perfect_date(year, mon, day);
}

const int Day::__change_money(const std::string category_name, const int cost, const std::string time, const std::string text, bool type)
{
	Category* temp = __find_category(category_name);
	int index = _max_sereial_number++;
	_history_money[index] = cost;
	_history_time[index] = time;
	_history_text[index] = text;

	if (type == INCOME)
	{
		_total_money += cost;
		_history_type[index] = INCOME;
		temp->__add_income(cost, text, time);
	}
	else
	{
		_total_money -= cost;
		_history_type[index] = EXPENSE;
		temp->__add_expense(cost, text, time);
	}


}
///-------------------------------------------------------------------------------------------

/*
	[ Return Value ]
		{-1} : failed to create day : same day have already exist
		{0} : Success to create day

	[ Function Act ]
		Create day.
*/
const int __create_day(int year, int mon, int day)
{
	std::string date = __make_perfect_date(year, mon, day);
	std::vector<Day*>::iterator iter = day_ptrs.begin();
	for (; iter != day_ptrs.end(); iter++)
	{
		if ((*iter)->_date == date)
		{
			return -1;
		}
	}

	Day* temp_Day = new Day(date);
	day_ptrs.push_back(temp_Day);
	return 0;
}