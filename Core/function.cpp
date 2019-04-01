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
		buffer : 검사할 버퍼

	Return
		가장 처음의 인덱스 부터 X번쨰 인덱스까지 중, 시작 인덱스에서 끊어지지 않고
		만들어 질수 있는 가장 큰 수
*/
const int __get_num_from_string( char* buffer)
{
	int cnt = 0;
	char* temp_buffer = NULL;

	for (int i = 0; i < strlen(buffer); i++)
	{
		if (buffer[i] < 48 || buffer[i]>57)
		{
			cnt = i;
			temp_buffer = new char[cnt+1];
			break;
		}
	}

	for (int i = 0; i < cnt; i++)
	{
		temp_buffer[i] = buffer[i];
	}
	temp_buffer[cnt] = '\0';

	int num = 0;
	for (int i = strlen(temp_buffer)-1, times = 1; i >= 0; i--, times *= 10)
	{
		num+= (temp_buffer[i] - 48)*times;
	}

	delete[] temp_buffer;
	return num;
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
		day : 날짜

	Return
		string : 두자리로 고정된 날짜
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
		reset_day : 초기화 날짜

	Return
		다음 초기화 날짜
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

