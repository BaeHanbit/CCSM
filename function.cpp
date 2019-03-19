#include "function.h"

/*
	[ Return Value]
		{28~31} : Normal

	[ Function Act ]
		Return Maximum day of the month inputed.
*/
const int __maximum_day(int month, int year)
{
	if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
		return 31;
	else if (month == 4 || month == 6 || month == 9 || month == 11)
		return 30;
	else if (month == 2 && year % 400 == 0)
		return 29;
	else if (month == 2 && year % 100 == 0)
		return 28;
	else if (month == 2 && year % 4 == 0)
		return 29;
	else
		return 28;
}

std::string __make_perfect_date(int year, int mon, int day)
{
	std::string str_year, str_mon, str_day;
	
	str_year = std::to_string(year);
	str_mon = std::to_string(mon);
	str_day = std::to_string(day);

	if (str_mon.length() == 1)
	{
		str_mon.insert(0, "0");
	}
	if (str_day.length() == 1)
	{
		str_day.insert(0, "0");
	}

	return std::string(str_year + "-" + str_mon + "-" + str_day);
}

/*
	[ Return Value ]
		{0~6} : Normal
		{100} : when error has been occured

	[ Function Act ]
		Return next weekday value
*/
weekday __next_weekday(weekday temp_day)
{
	if (temp_day == SUNDAY)
		return MONDAY;
	else if (temp_day == MONDAY)
		return TUESDAY;
	else if (temp_day == TUESDAY)
		return WEDNESDAY;
	else if (temp_day == WEDNESDAY)
		return THURSDAY;
	else if (temp_day == THURSDAY)
		return FRIDAY;
	else if (temp_day == FRIDAY)
		return SATURDAY;
	else if (temp_day == SATURDAY)
		return SUNDAY;
	else
		return WEEKDAY_ERROR;
}

/*
	[ Return Value]
		{0} : Normal

	[ Function Act ]
		Match weekday and date.
		Set it in calender map.
*/
const int __set_calender()
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	weekday weekday = SATURDAY;
	std::string temp_date = "2000-01-01";

	for (int temp_year = 2000; temp_year <= tm.tm_year + 1901; temp_year++)
	{
		for (int temp_month = 1; temp_month <= 12; temp_month++)
		{
			for (int temp_day = 1; temp_day <= __maximum_day(temp_month, temp_year); temp_day++)
			{
				temp_date = __make_perfect_date(temp_year, temp_month, temp_day);
				calender.insert(std::make_pair(weekday, temp_date));
				std::cout << temp_date<<"   "<<weekday<<std::endl;

				weekday = __next_weekday(weekday);
			}
			std::cout << std::endl;
		}
	}
	return 0;
}

/*
	[ Return Value]
		string : today :date

	[ Function Act ]
		return today date
*/
std::string __today_date()
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	return __make_perfect_date(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}