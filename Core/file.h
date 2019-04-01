#pragma once
#pragma warning(disable:4996)

#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include <list>
#include <vector>
#include <time.h>
#include <Windows.h>
#include <direct.h>
#include <io.h>
#include <stdio.h>

enum type { INCOME = 0, EXPENSE };
enum flag { NOTNEED=0,NEED};

const int __create_dir(std::string name);
const bool __find_dir(std::string name);

const int __create_day(int year, int mon, int day);
const bool __find_day(int year, int mon, int day);
void __add_record_day(std::string cate, int cost, std::string text, std::string time, bool type, int index);

const int __create_master_cate();
const int __cate_cnt();
const int __create_cate(std::string name, int reset_day);
const bool __find_cate(std::string name);
void __remove_cate(std::string name);
void __get_cate_names(std::string*);
void __add_record_cate(std::string name, int cost, std::string text, std::string time, bool type);
void __get_cate_data(std::string* buffer, std::string cate_name, int cnt=1, int start_point=1);
void __remove_record_cate(std::string cate, int index);

const bool __set_flag(std::string name, bool flag);
const bool __check_file_exist(std::string);

