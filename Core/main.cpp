#include "file.h"
#include "function.h"

enum menu{CREATE_CATEGORY=0,REMOVE_CATEGORY,CATEGORY_COUNT};

void main()
{
	__create_dir(std::string("Category"));
	__create_dir(std::string("Day"));
	__create_master_cate();
	//__create_cate("식비", 1);
	//__create_cate("관리비", 31);
	__create_day(2000, 11, 20);
	__create_day(2001, 11, 20);

	std::string* temp = new std::string[__cate_cnt()];
	__get_cate_names(temp);

	for (int i = 0; i < __cate_cnt(); i++)
	{
		std::cout << temp[i] << std::endl;
	}
	std::cout << "--------------------------------\n";
	
	__remove_cate("식비");
	__remove_cate("20");
	delete[] temp;
}