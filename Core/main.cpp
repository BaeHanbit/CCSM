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
	//__Remove_category_dir("�ĺ�");
	//__Remove_category_file("�ĺ�");

	//std::string temp[100];
	//int cnt = __Get_all_file(temp,"./Category/�ĺ�");

	//for (int i = 0; i < cnt; i++)
	//{
	//	std::cout << temp[i] << std::endl;
	//}
}