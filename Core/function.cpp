#include "function.h"
#include "file.h"

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

/*
	다음달을 리턴한다.
	만약 12월이 입력되었다면 13월이 아닌 1월이 리턴된다.
*/
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
		-1 : 들어온 버퍼가 빔
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
	Parameter
		month : 월

	Return
		string : 두자리로 고정된 월
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
	Get year, mon, day data through parameter
	Return date string with YYYY-MM-DD format.
*/
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

/*
	Return time in HH-MM format
	hour format is 24
*/
std::string __current_time()
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	std::string min = std::to_string(tm.tm_min);
	std::string hour = std::to_string(tm.tm_hour);

	return hour + '-' + min;
}

/*
	Return : 입력받은 record 데이터에서 해당 레코드의 기록 일을 반환한다.
	이때 시간을 포함한다
*/
std::string __Return_time_from_record(std::string record)
{
	//  index|date+time|memo|+/-cost
	std::string date;
	for (int i = 0; i < record.length(); i++)
	{
		if (record[i] == '|')
		{
			for (int j = i + 1; j < record.length(); j++)
			{
				date += record[j];
				if (record[j] == '|')
				{
					return date;
				}//end of 2nd if
			}//end of 2nd for
		}//end of 1st if
	}//end of 1st for
	throw true;
}

/*
	문자열로 이루어진 YYYY-MM데이터를 받아 다음 월을 같은 형식으로 리턴한다.
*/
std::string __Next_mon_string(std::string mon)
{
	if (mon.substr(5, 2) == "12")
		return std::string(std::to_string(std::stoi(mon.substr(0, 4)) + 1) + "-01");
	else
		return std::string(mon.substr(0, 5) + __make_perfect_month(std::stoi(mon.substr(5, 2)) + 1));
}

/*
	해당 카테고리의 시작 월부터 끝나는 월 안에서 가장 시작월과 가까운 존재하는 파일의 이름을 리턴한다.
	없다면 -1
*/
std::string __Get_nearest_mon_exist_file(std::string category_name, std::string start_mon, std::string end_mon)
{
	std::string location = CATEGORY + category_name + "/" + start_mon + ".txt";
	std::string month_point = start_mon;
	do
	{
		if (__File_exist(location))
		{
			return month_point;
		}
		else
		{
			month_point = __Next_mon_string(month_point);
			location = CATEGORY + category_name + "/" + month_point + ".txt";
		}
	} while ((start_mon < end_mon));

	if (start_mon >= end_mon)
		throw - 1;
}

/*
	date 1이 더 빠른 날짜면 1을 리턴
	date 2가 더 빠른 날짜면 2를 리턴
	같으면 0

	연도의 차이가 자리수 이상이면 -1
*/
int __Compare_date(std::string date1, std::string date2)
{
	if (date1.length() != date2.length())
		return -1;

	for (int i = 0; i < date1.length(); i++)
	{
		if (i == 4 || i == 7)
			continue;
		else
		{
			if (date1[i] > date2[i])
			{
				return 2;
			}
			else if (date1[i] < date2[i])
			{
				return 1;
			}
			else
			{
				continue;
			}
		}
	}
	return 0;
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

