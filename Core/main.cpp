#include "file.h"
#include "function.h"

enum menu{CREATE_CATEGORY=0,REMOVE_CATEGORY,CATEGORY_COUNT};
void main()
{
	__Create_index_file();
	__Create_dir(CATEGORY);
	

	__Create_category("�ĺ�", 10);
	__Create_category("�����", 1);
	__Create_category("�������", 31);


	__Insert_cate_data("�ĺ�", __today_date() + '-' + __current_time(), "����1", 10, INCOME);
	__Insert_cate_data("�ĺ�", __today_date() + '-' + __current_time(), "����2", 100, INCOME);
	__Insert_cate_data("�ĺ�", __today_date() + '-' + __current_time(), "����3", 1000, INCOME);
	__Insert_cate_data("�ĺ�", __today_date() + '-' + __current_time(), "����4", 10000, INCOME);
	__Insert_cate_data("�ĺ�", __today_date() + '-' + __current_time(), "����5", 10, INCOME);
	__Insert_cate_data("�ĺ�", __today_date() + '-' + __current_time(), "����6", 100, INCOME);
	__Insert_cate_data("�ĺ�", __today_date() + '-' + __current_time(), "����7", 1000, INCOME);
	__Insert_cate_data("�ĺ�", __today_date() + '-' + __current_time(), "����0", 10, EXPENSE);
	__Insert_cate_data("�ĺ�", __today_date() + '-' + __current_time(), "����1", 100, EXPENSE);
	__Insert_cate_data("�ĺ�", __today_date() + '-' + __current_time(), "����2", 1000, EXPENSE);
	__Insert_cate_data("�ĺ�", __today_date() + '-' + __current_time(), "����3", 10000, EXPENSE);
	__Insert_cate_data("�ĺ�", __today_date() + '-' + __current_time(), "����4", 10, EXPENSE);
	__Insert_cate_data("�ĺ�", __today_date() + '-' + __current_time(), "����5", 100, EXPENSE);
	__Insert_cate_data("�ĺ�", __today_date() + '-' + __current_time(), "����6", 1000, EXPENSE);


	//std::cout << __Remove_cate_data("�ĺ�", "2019-04", 5) << std::endl;
	//std::cout << __Remove_cate_data("�ĺ�", "2019-04", 44) << std::endl;


	//__Remove_category_dir("�ĺ�");
	//__Remove_category_file("�ĺ�");


	std::string* temp = new std::string[__Category_month_file_cnt("�ĺ�")];
	int cnt = __Get_all_file(temp,"./Category/�ĺ�");
	for (int i = 0; i < cnt; i++)
	{
		std::cout << temp[i] << std::endl;
	}
	delete[] temp;
}