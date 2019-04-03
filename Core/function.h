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
const int __get_num_from_string(std::string buffer);
std::string __make_perfect_date(int tm_year, int tm_mon, int tm_day);
std::string __make_perfect_day(int day);
std::string __make_perfect_month(int mon);
std::string __today_date();
std::string __next_reset_date(int reset_day);
std::string __current_time();
std::string __Return_time_from_record(std::string record);
std::string __Get_nearest_exist_file(std::string category_name, std::string start_mon, std::string end_mon);

int __Compare_date(std::string date1, std::string date2);
weekday __next_weekday(weekday temp_day);
