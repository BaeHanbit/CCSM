#pragma once
#include "header.h"

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
void __Remove_category(std::string name);
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
const int __Get_total_from_to(std::string category_name, std::string start_date, std::string end_date, bool flag);
const int __Get_data(std::string* data_buffer, std::string category_name, std::string which_month, int start_index = 0, int cnt = 1);

