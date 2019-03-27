#pragma once
#pragma warning(disable:4996)
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <time.h>
#include <fstream>

enum weekday { SUNDAY = 0, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, WEEKDAY_ERROR = 100 };
static std::map<int, std::string>calender;

const int __maximum_day(int month,int year);
std::string __make_perfect_date(int tm_year, int tm_mon, int tm_day);
weekday __next_weekday(weekday temp_day);
const int __set_calender();
std::string __today_date();
const int __next_month(int mon);
std::string __next_reset_date(int reset_day);

