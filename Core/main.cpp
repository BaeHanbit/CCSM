#include "category.h"
#include "day.h"
#include "function.h"

enum menu{CREATE_CATEGORY=0,REMOVE_CATEGORY,CATEGORY_COUNT};
extern std::list<std::pair<Category*, std::string>>cate_ptrs;
int main()
{
	int menu = 0;
	int state = 0;
	std::string name = { 0 };
	std::string date = { 0 };
	do
	{
		std::cout << std::endl << std::endl;
		std::cout<<"[1. Create Category | 2. Remove Category | 3. Category Counts | 4.set_calender]\nInput : ";
		std::cin >> menu;
		system("cls");
		switch (menu)
		{
		case 1:
		{
			state = __create_category();
			break;
		}

		case 2:
		{
			state = __remove_category();
			break;
		}
		case 3:
		{
			std::string temp;
			std::cout << "Plz input category name which you wanna find : ";
			std::cin >> temp;
			const Category* gotten_category = __find_category(temp);
			
			for (std::list<std::pair<Category*, std::string>>::iterator iter = cate_ptrs.begin();
				iter != cate_ptrs.end(); iter++)
			{
				if (gotten_category == iter->first)
					std::cout << iter->second << std::endl;
			}
			break;
		}
		case 4:
		{
			__set_calender();
			break;
		}

		default:
		{
			printf("plz input correct key!!!");
			break;
		}
		}//end of switch

	} while (menu != 0);

	return 0;
}