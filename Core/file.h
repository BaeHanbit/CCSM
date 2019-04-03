#pragma once
#pragma warning(disable:4996)
#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include <filesystem>
#include <experimental/filesystem>
#include <list>
#include <vector>
#include <time.h>
#include <Windows.h>
#include <direct.h>
#include <io.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <errno.h>

#define CATEGORY std::string("./Category/")
#define DAY std::string("./Day/")
#define MASTER std::string("./Master/")
#define MAX_MEMO_SIZE 50
#define MAX_CATEGORY_NUMBER 20
enum type { INCOME = 0, EXPENSE };
enum flag { NOTNEED=0,NEED};


const bool __File_exist(std::string location);
const bool __Find_dir(std::string name);
const int __Create_dir(std::string name);
void __Create_category(std::string name, int reset_date);
void __Create_txt_file(std::string name);
void __Remove_category_dir(std::string name);
void __Remove_category_file(std::string name);
int __Get_all_file(std::string* buffer, std::string location);
const int __Category_month_file_cnt(std::string category_name);
void __Insert_cate_data(std::string category_name, std::string time, std::string memo, int cost, bool type);
int __Remove_cate_data(std::string category_name, std::string which_month, int index);


void __Create_index_file();
int __Get_index();
void __Set_index(int index);


int __Get_total_income(std::string category_name, std::string which_month);
void __Set_total_income(std::string category_name, std::string which_month, int cost);
int __Get_total_expense(std::string category_name, std::string which_month);
void __Set_total_expense(std::string category_name, std::string which_month, int cost);



const int __create_day(int year, int mon, int day);
const bool __find_day(int year, int mon, int day);
void __add_record_day(std::string cate, int cost, std::string text, std::string time, bool type, int index);


const bool __set_flag(std::string name, bool flag);

