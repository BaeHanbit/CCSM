#pragma once
#pragma warning(disable:4996)

#include <iostream>
#include <string>
#include <iterator>
#include <time.h>
#include <map>
#include <fstream>
#include <Windows.h>
#include <direct.h>
#include <io.h>

const int __create_dir(std::string name);
const bool __find_dir(std::string name);

const int __create_day(int year, int mon, int day);
const bool __find_day(int year, int mon, int day);

const int __create_cate(std::string name, int reset_day);
const bool __find_cate(std::string name);

const bool __check_file_exist(std::string);