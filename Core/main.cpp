#include "file.h"
#include "function.h"

enum menu{CREATE_CATEGORY=0,REMOVE_CATEGORY,CATEGORY_COUNT};

void main()
{
	__create_dir(std::string("Category"));
	__create_dir(std::string("Day"));
	__create_master_cate();
	__create_cate("½Äºñ", 1);
	__create_cate("°ü¸®ºñ", 31);
	__create_day(2000, 11, 20);
	__create_day(2001, 11, 20);

	__add_record_cate("½Äºñ", 1000, "±è¹ä", "2019-03-31-18-02", INCOME);
	__add_record_cate("½Äºñ", 2000, "±è¹ä", "2019-03-31-18-02", INCOME);

	__remove_cate("½Äºñ");
	/*
	std::string* temp = new std::string[__cate_cnt()];
	__get_cate_names(temp);
	__remove_cate("½Äºñ");
	delete[] temp;
	*/
}