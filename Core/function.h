#pragma once
#include "header.h"

enum weekday { SUNDAY = 0, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, WEEKDAY_ERROR = 100 };
extern std::vector<std::pair<std::string, weekday>> calender;

const int __Maximum_day(int month,int year);
const int __next_month(int mon);
void __Set_calender();
const int __get_num_from_string(std::string buffer);


std::string __make_perfect_day(int day);
std::string __make_perfect_month(int mon);
std::string __make_perfect_date(int tm_year, int tm_mon, int tm_day);
std::string __today_date();
std::string __next_reset_date(int reset_day);
std::string __current_time();
std::string __Return_time_from_record(std::string record);
std::string __Next_mon_string(std::string mon);
std::string __Get_nearest_mon_exist_file(std::string category_name, std::string start_mon, std::string end_mon);

int __Compare_date(std::string date1, std::string date2);
weekday __next_weekday(weekday temp_day);
