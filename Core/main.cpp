#include "file.h"
#include "function.h"

enum menu{CREATE_CATEGORY=0,REMOVE_CATEGORY,CATEGORY_COUNT};
void main()
{
	__Create_index_file();
	__Create_dir(CATEGORY);
	

	__Create_category("식비", 10);
	__Create_category("교통비", 1);
	__Create_category("경조사비", 31);


	__Insert_cate_data("식비", __today_date() + '-' + __current_time(), "수입1", 10, INCOME);
	__Insert_cate_data("식비", __today_date() + '-' + __current_time(), "수입2", 100, INCOME);
	__Insert_cate_data("식비", __today_date() + '-' + __current_time(), "수입3", 1000, INCOME);
	__Insert_cate_data("식비", __today_date() + '-' + __current_time(), "수입4", 10000, INCOME);
	__Insert_cate_data("식비", __today_date() + '-' + __current_time(), "수입5", 10, INCOME);
	__Insert_cate_data("식비", __today_date() + '-' + __current_time(), "수입6", 100, INCOME);
	__Insert_cate_data("식비", __today_date() + '-' + __current_time(), "수입7", 1000, INCOME);
	__Insert_cate_data("식비", __today_date() + '-' + __current_time(), "지출0", 10, EXPENSE);
	__Insert_cate_data("식비", __today_date() + '-' + __current_time(), "지출1", 100, EXPENSE);
	__Insert_cate_data("식비", __today_date() + '-' + __current_time(), "지출2", 1000, EXPENSE);
	__Insert_cate_data("식비", __today_date() + '-' + __current_time(), "지출3", 10000, EXPENSE);
	__Insert_cate_data("식비", __today_date() + '-' + __current_time(), "지출4", 10, EXPENSE);
	__Insert_cate_data("식비", __today_date() + '-' + __current_time(), "지출5", 100, EXPENSE);
	__Insert_cate_data("식비", __today_date() + '-' + __current_time(), "지출6", 1000, EXPENSE);


	//std::cout << __Remove_cate_data("식비", "2019-04", 5) << std::endl;
	//std::cout << __Remove_cate_data("식비", "2019-04", 44) << std::endl;


	//__Remove_category_dir("식비");
	//__Remove_category_file("식비");


	std::string* temp = new std::string[__Category_month_file_cnt("식비")];
	int cnt = __Get_all_file(temp,"./Category/식비");
	for (int i = 0; i < cnt; i++)
	{
		std::cout << temp[i] << std::endl;
	}
	delete[] temp;
}