#include "file.h"
#include "function.h"

enum menu{CREATE_CATEGORY=0,REMOVE_CATEGORY,CATEGORY_COUNT};
void main()
{
	__Create_index_file();
	__Create_dir(CATEGORY);
	
	//__Create_category("�ĺ�", 10);
	//__Create_category("�����", 1);
	//__Create_category("�������", 31);

	__Insert_cate_data("�ĺ�", __today_date() + '-' + __current_time(), "���� ����", 800, EXPENSE);
	__Insert_cate_data("�ĺ�", __today_date() + '-' + __current_time(), "�ް�", 5000, EXPENSE);
	__Insert_cate_data("�ĺ�", __today_date() + '-' + __current_time(), "��!", 800, EXPENSE);
	__Insert_cate_data("�ĺ�", __today_date() + '-' + __current_time(), "��â", 30000, EXPENSE);


	//__Remove_category_dir("�ĺ�");
	//__Remove_category_file("�ĺ�");

	//std::string temp[100];
	//int cnt = __Get_all_file(temp,"./Category/�ĺ�");

	//for (int i = 0; i < cnt; i++)
	//{
	//	std::cout << temp[i] << std::endl;
	//}
}