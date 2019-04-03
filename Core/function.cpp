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


const int __next_month(int mon)
{
	return mon + 1 == 12 ? 1 : mon + 2;
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
	Parameter
		buffer : �˻��� ����

	Return
		���� ó���� �ε��� ���� X���� �ε������� ��, ���� �ε������� �������� �ʰ�
		����� ���� �ִ� ���� ū ��
		-1 : ���� ���۰� ��
*/
const int __get_num_from_string(std::string buffer)
{
	if (buffer.length() == 0)
	{
		return -1;
	}
	else
	{
		std::string pure_num_string;
		for (int i = 0; i < buffer.length(); i++)
		{
			if (buffer[i] < 48 || buffer[i]>57)
			{
				break;
			}
			else
			{
				pure_num_string += buffer[i];
			}
		}
		return std::stoi(pure_num_string);
	}
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
	Parameter
		day : ��¥

	Return
		string : ���ڸ��� ������ ��¥
*/
std::string __make_perfect_day(int day)
{
	if (day < 10)
	{
		return '0' + std::to_string(day);
	}
	else
		return std::to_string(day);
}

/*
	Parameter
		month : ��

	Return
		string : ���ڸ��� ������ ��
*/
std::string __make_perfect_month(int mon)
{
	if (mon < 10)
	{
		return '0' + std::to_string(mon);
	}
	else
		return std::to_string(mon);
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

/*
	Parameter
		reset_day : �ʱ�ȭ ��¥

	Return
		���� �ʱ�ȭ ��¥
*/
std::string __next_reset_date(int reset_day)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);



	int next_mon = __next_month(tm.tm_mon);
	int year = next_mon == 1 ? tm.tm_year + 1901 : tm.tm_year + 1900;
	int max_day = __maximum_day(next_mon, year);

	if (tm.tm_mday < reset_day)
	{
		max_day = __maximum_day(tm.tm_mon + 1, tm.tm_year + 1900);
		return __make_perfect_date(tm.tm_year + 1900, tm.tm_mon + 1, reset_day > max_day ? max_day : reset_day);
	}
	else
	{
		if (max_day >= reset_day)
		{
			return __make_perfect_date(year, next_mon, reset_day);
		}
		else
		{
			return __make_perfect_date(year, next_mon, max_day);
		}
	}
}

std::string __current_time()
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	std::string min = std::to_string(tm.tm_min);
	std::string hour = std::to_string(tm.tm_hour);

	return hour + '-' + min;
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

