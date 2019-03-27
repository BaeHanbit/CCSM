#include "file.h"
#include "function.h"

enum menu{CREATE_CATEGORY=0,REMOVE_CATEGORY,CATEGORY_COUNT};

void main()
{
	__create_dir(std::string("Category"));
	__create_dir(std::string("Day"));
	
	__create_cate("Cate01",28);
	__create_day(2100, 2, 29);
}