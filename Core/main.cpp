#include "file.h"
#include "function.h"

enum menu{CREATE_CATEGORY=0,REMOVE_CATEGORY,CATEGORY_COUNT};
void main()
{
	__Create_index_file();
	__Create_dir(CATEGORY);
	
	//__Create_category("식비", 10);
	//__Create_category("교통비", 1);
	//__Create_category("경조사비", 31);

	__Insert_cate_data("식비", __today_date() + '-' + __current_time(), "팽이 버섯", 800, EXPENSE);
	__Insert_cate_data("식비", __today_date() + '-' + __current_time(), "달걀", 5000, EXPENSE);
	__Insert_cate_data("식비", __today_date() + '-' + __current_time(), "와!", 800, EXPENSE);
	__Insert_cate_data("식비", __today_date() + '-' + __current_time(), "곱창", 30000, EXPENSE);


	//__Remove_category_dir("식비");
	//__Remove_category_file("식비");

	//std::string temp[100];
	//int cnt = __Get_all_file(temp,"./Category/식비");

	//for (int i = 0; i < cnt; i++)
	//{
	//	std::cout << temp[i] << std::endl;
	//}
}