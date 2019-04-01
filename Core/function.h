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
const int __next_month(int mon);
const int __set_calender();
const int __get_num_from_string( char* buffer);
std::string __make_perfect_date(int tm_year, int tm_mon, int tm_day);
std::string __make_perfect_day(int day);
std::string __today_date();
std::string __next_reset_date(int reset_day);
std::string __current_time();
weekday __next_weekday(weekday temp_day);
