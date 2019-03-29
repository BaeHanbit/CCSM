#include "file.h"
#include "function.h"

enum menu{CREATE_CATEGORY=0,REMOVE_CATEGORY,CATEGORY_COUNT};

void main()
{
	__create_dir(std::string("Category"));
	__create_dir(std::string("Day"));
	__create_master_cate();
	__create_cate("½Äºñ", 28);
	__create_cate("Cate02", 28);

	std::string* temp = new std::string[__cate_cnt()];
	__get_cate_names(temp);

	for (int i = 0; i < __cate_cnt(); i++)
	{
		std::cout << temp[i] << std::endl;
	}


	delete[] temp;
}