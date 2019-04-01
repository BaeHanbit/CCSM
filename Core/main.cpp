#include "file.h"
#include "function.h"

enum menu{CREATE_CATEGORY=0,REMOVE_CATEGORY,CATEGORY_COUNT};

void main()
{
	std::string buffer[100];
	std::string date = __today_date();
	std::string time = __current_time();

	__create_dir(std::string("Category"));
	__create_dir(std::string("Day"));
	__create_master_cate();
	__create_cate("½Äºñ", 1);
	__create_cate("°ü¸®ºñ", 31);
	__create_day(2019, 3, 31);

	__add_record_cate("½Äºñ", 1000, "ÆØÀÌ¹ö¼¸", __current_time(), EXPENSE);
	__add_record_cate("½Äºñ", 1000, "ÆØÀÌ¹ö¼¸", __current_time(), EXPENSE);
	__add_record_cate("½Äºñ", 1000, "ÆØÀÌ¹ö¼¸", __current_time(), EXPENSE);
	__add_record_cate("½Äºñ", 1000, "ÆØÀÌ¹ö¼¸", __current_time(), EXPENSE);
	__add_record_cate("½Äºñ", 1000, "ÆØÀÌ¹ö¼¸", __current_time(), EXPENSE);
	__add_record_cate("½Äºñ", 1000, "ÆØÀÌ¹ö¼¸", __current_time(), EXPENSE);

	__remove_record_cate("½Äºñ", 2);
}