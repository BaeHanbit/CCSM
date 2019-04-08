#include "file.h"
#include "function.h"

/*
	�ش� ���� �����ϸ� 1
	������ 0 ����
*/
const bool __File_exist(std::string location)
{
	std::ifstream f(location.c_str());
	if (f.good()) {
		f.close();
		return true;
	}
	else {
		f.close();
		return false;
	}
}

/*
	�ش� ī�װ��� �����ϴ��� Ȯ���մϴ�.
*/
const bool __Find_dir(std::string name)
{
	return access(name.c_str(), 00) == 0 ? 1 : 0;
}

/*
	���ڷ� ���� �̸��� ���丮�� ����ϴ�.
*/
const int __Create_dir(std::string name)
{
	if (__Find_dir(name))
	{
		return -1;
	}
	else
	{
		_mkdir(name.c_str());
		return 0;
	}
}

/*
	Category ���� �ؿ� �ڽŸ��� ���� ����
	�ڽŸ��� ���� �ؿ� ���� ���� ���� ����
	Master ���� �Ʒ� �ڽ��� ��Ÿ �����͸� ������ ���� ����
*/
void __Create_category(std::string name, int reset_date)
{
	__Create_dir(MASTER);
	__Create_dir(CATEGORY + name);

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	std::string file_name = CATEGORY + name + "/" + std::to_string(tm.tm_year + 1900) + '-' + __make_perfect_month(tm.tm_mon + 1);
	__Create_txt_file(file_name);
	__Create_txt_file(MASTER + name);

	file_name += ".txt";
	std::ofstream wFile(file_name, std::ios::in | std::ios::out);
	wFile.seekp(0, std::ios::beg);

	//Category month file ��� : Total Income | Total Expense
	std::string content = "0|0                      ";
	wFile << content << std::endl;

	std::string master_file = MASTER + name + ".txt";
	std::ofstream wFile2(master_file);

	//Master File ��� : Reset_date | Date when Category was made
	content = __make_perfect_day(reset_date) + '|' + __make_perfect_date(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
	wFile2 << content << std::endl;
}

/*
	���ڷ� ���� ��ο� �̸��� ���� ����
*/
void __Create_txt_file(std::string name)
{
	std::string file_name = name + ".txt";
	if (__File_exist(file_name))
	{
		return;
	}
	else
	{
		std::ofstream make_file(file_name);
	}
}

/*
	�ش� ī�װ� ������ ����ϴ�.
	�̶� Master�� �ڱ� �ڽ��� ������ �����ؾ� �մϴ�.
*/
void __Remove_category_dir(std::string name)
{
	std::string directory_location = CATEGORY + name;

	if (rmdir(directory_location.c_str()) != -1)
		return;

	else
	{
		std::string master_file_location = MASTER + name + ".txt";
		std::ifstream rFile(master_file_location, std::ios::in | std::ios::out);

		std::string s_year;
		std::string s_mon;
		
		char temp[100] = { 0 };
		rFile.getline(temp, 100);
		
		for (int i = 3; i <= 6; i++)
		{
			 s_year += temp[i];
		}
		for (int i = 8; i <= 9; i++)
		{
			s_mon += temp[i];
		}
		int year = std::stoi(s_year);
		int month = std::stoi(s_mon);

		time_t t = time(NULL);
		struct tm tm = *localtime(&t);

		int current_year = tm.tm_year + 1900;
		int current_mon = tm.tm_mon + 1;
		int cnt_of_txt = (current_year - year) * 12 - month + current_mon + 1;
		std::string* buffer = new std::string[cnt_of_txt*10];
		
		int cnt = __Get_all_file(buffer, directory_location);
		
		for (int i = 0; i < cnt_of_txt; i++)
		{
			std::string file_location = directory_location + "/" + buffer[i];
			std::remove(file_location.c_str());
		}
		rmdir(directory_location.c_str());
		delete[] buffer;
	}
}

/*
	Master���� �ؿ� �����ϴ� �ڽ��� ������ ����ִ� ���� ����
*/
void __Remove_category_file(std::string name)
{
	std::string location = MASTER + name + ".txt";
	std::remove(location.c_str());
}

/*
	ī�װ� ���ϰ� ī�װ� ���� ����
*/
void __Remove_category(std::string name)
{
	__Remove_category_dir(name);
	__Remove_category_file(name);
}

/*
	�Է� ���� ���ۿ�, �ش� ���(���丮)���� ��� txt���� ������ �����
	�̶� ���۸� �迭�� ��������, �迭 ũ�⸦ �˾ƾ���
*/
int __Get_all_file(std::string* buffer, std::string location)
{
	_finddatai64_t c_file;
	intptr_t hFile;
	std::string temp_location = location + "/*.txt";
	int cnt = 0;

	if ((hFile = _findfirsti64(temp_location.c_str(), &c_file)) == -1L) 
	{
		return -1;
	} 
	else
	{
		do
		{
			buffer[cnt++] = c_file.name;
		} while (_findnexti64(hFile, &c_file) == 0);
		_findclose(hFile);
	} 

	return cnt;
}

/*

*/
const int __Category_month_file_cnt(std::string category_name)
{
	std::string location = MASTER +category_name + ".txt";
	std::ifstream rFile(location, std::ios::in);
	rFile.seekg(0, std::ios::beg);

	std::string buffer;
	std::getline(rFile, buffer);

	int made_year = std::stoi(buffer.substr(0, 4));
	int made_month = std::stoi(buffer.substr(5, 2));
	int today_year = std::stoi(__today_date().substr(0, 4));
	int today_month = std::stoi(__today_date().substr(5, 2));

	return (today_year - made_year) * 12 + today_month - made_month;
}

/*
	�ش� ī�װ��� ����,���� ����� �߰��Ѵ�	
*/
void __Insert_cate_data(std::string category_name, std::string time, std::string memo, int cost, bool type)
{
	std::string today = __today_date();
	today = today.substr(0, 7);
	std::string location = CATEGORY + category_name + "/" + today;

	if (__File_exist(location + ".txt")){}
	else
	{
		__Create_txt_file(location);
		std::ofstream wFile(location + ".txt");
		wFile << "0|0                      " << std::endl;
		wFile.close();
	}


	location += ".txt";
	std::ofstream wFile(location, std::ios::in | std::ios::out);

	int index = __Get_index();
	__Set_index(++index);

	char symbol = type == INCOME ? '+' : '-';
	//content ��� : index | time | memo | type+cost
	std::string content = std::to_string(index) + "|" + time + "|" + memo + "|" + symbol + std::to_string(cost);

	wFile.seekp(0, std::ios::end);
	wFile << content << std::endl;
	wFile.seekp(0, std::ios::beg);
	
	//Category month file ��� : Total Income | Total Expense
	if (type == INCOME)
	{
		int temp = __Get_total_income(category_name, today);
		__Set_total_income(category_name, today, temp+cost);
	}
	else
	{
		int temp = __Get_total_expense(category_name, today);
		__Set_total_expense(category_name, today, temp + cost);
	}

	wFile.close();
}

/*
	Return
		-1 : �ش� �ε����� �ش�Ǵ� �� ����
		�ش� �ε��� ��ȣ ���
*/
int __Remove_cate_data(std::string category_name, std::string which_month, int index)
{
	std::string location = CATEGORY + category_name + "/" + which_month + ".txt";
	std::ifstream rFile(location, std::ios::in);
	rFile.seekg(0, std::ios::beg);
	

	std::vector<std::string> record_list;
	std::string buffer;
	int delete_point = 0;
	int temp_delete_point = 0;
	bool flag_save = 0;
	std::getline(rFile, buffer); //Metadata get


	for (int i = 0, buffer_index = 0; !rFile.eof(); i++)
	{
		if (flag_save==false)
		{
			temp_delete_point = rFile.tellg();
		}
		std::getline(rFile, buffer);
		if (flag_save == false)
		{
			buffer_index = __get_num_from_string(buffer);
		}


		if (flag_save == true)
		{
			record_list.push_back(buffer);
		}
		else if (buffer_index == index)
		{
			delete_point = temp_delete_point;
			flag_save = true;
		}
		else if (i == 0 && buffer_index > index)
		{
			return -1;
		}
	}
	rFile.close();


	if (flag_save==false)
	{
		return -1;
	}
	else
	{
		FILE *fp = fopen(location.c_str(), "a+");
		chsize(fileno(fp), delete_point);
		fclose(fp);


		std::ofstream wFile(location, std::ios::in | std::ios::out);
		wFile.seekp(0, std::ios::end);
		for (int i = 0; i < record_list.size(); i++)
		{
			wFile << record_list[i] << std::endl;
		}


		wFile.close();
		return index;
	}
	return -1;
}

/*
	������ȣ�� ������ index������ �����Ѵ�.
*/
void __Create_index_file()
{
	std::string location = "./Index.txt";
	if (__File_exist(location))
	{
		return;
	}
	else
	{
		__Create_txt_file("Index");
		std::ofstream wFile("./Index.txt", std::ios::in || std::ios::out);
		wFile << std::to_string(0);
	}
}

/*
	index ��ȣ�� �����´�.
*/
int __Get_index()
{
	char buffer[100];
	std::ifstream rFile("./Index.txt", std::ios::in | std::ios::out);
	rFile.getline(buffer, 100);
	return std::stoi(buffer);
}

/*
	index ��ȣ�� �����Ѵ�.
*/
void __Set_index(int index)
{
	std::ofstream wFile("./Index.txt", std::ios::in | std::ios::out);
	wFile.seekp(0, std::ios::beg);
	wFile << index;
}

/*
	ī�װ��� �ش��ϴ� ���� �� ������ �����Ѵ�.
*/
int __Get_total_income(std::string category_name, std::string which_month)
{
	std::string location = CATEGORY + category_name + "/" + which_month + ".txt";
	std::ifstream rFile(location);
	std::string income;
	std::getline(rFile, income);
	
	for (int i = 0; i < income.length(); i++)
	{
		if (income[i] == '|')
		{
			income = income.substr(0, i + 1);
			break;
		}
	}
	return std::stoi(income);
}

/*
	ī�װ��� �ش� ���� �� ���� ���� �Է��Ѵ�.
*/
void __Set_total_income(std::string category_name, std::string which_month, int cost)
{
	std::string location = CATEGORY + category_name + "/" + which_month + ".txt";
	std::ofstream wFile(location, std::ios::in | std::ios::out);
	std::ifstream rFile(location, std::ios::in);
	std::string buffer;
	std::string expense;
	std::string meta_data;
	std::vector<std::string> record_list;


	std::getline(rFile, buffer);
	for (int i = 0; i <=buffer.length(); i++)
	{
		if (buffer[i] == '|')
		{
			expense = buffer.substr(i + 1, buffer.length() - (i + 1));
			int temp_expense = std::stoi(expense);
			expense = std::to_string(temp_expense);
		}
	}


	meta_data = std::to_string(cost) + "|" + expense;
	if (meta_data.length() <= buffer.length())
	{
		while (meta_data.length() < buffer.length())
		{
			meta_data += ' ';
		}

		wFile.seekp(0, std::ios::beg);
		wFile << meta_data;
		wFile.close();
		rFile.close();
	}
	else
	{
		while (!rFile.eof())
		{
			std::getline(rFile, buffer);
			std::cout << buffer << std::endl;
			record_list.push_back(buffer);
		}
		wFile.close();

		std::ofstream newFile(location, std::ios::out);
		newFile << meta_data << std::endl;
		for (int i = 0; i < record_list.size(); i++)
		{
			newFile << record_list[i] << std::endl;
		}
		newFile.close();
		rFile.close();
	}
}

/*
	ī�װ��� �ش� ���� �� ������ ����Ѵ�.
*/
int __Get_total_expense(std::string category_name, std::string which_month)
{
	std::string location = CATEGORY + category_name + "/" + which_month + ".txt";
	std::ifstream rFile(location);
	std::string expense;
	std::getline(rFile, expense);

	for (int i = 0; i < expense.length(); i++)
	{
		if (expense[i] == '|')
		{
			expense = expense.substr(i + 1, expense.length() - (i + 1));
			break;
		}
	}
	return std::stoi(expense);
}

/*
	ī�װ��� �ش� �� ������ ���� ������ �����Ѵ�.
*/
void __Set_total_expense(std::string category_name, std::string which_month, int cost)
{
	std::string location = CATEGORY + category_name + "/" + which_month + ".txt";
	std::ofstream wFile(location, std::ios::in | std::ios::out);
	std::ifstream rFile(location,std::ios::in);
	std::string buffer;
	std::string income;
	std::string meta_data;
	std::vector<std::string> record_list;


	std::getline(rFile, buffer);
	for (int i = 0; buffer[i]!='|'; i++)
	{
			income += buffer[i];
	}
	int temp_income = std::stoi(income);
	income = std::to_string(temp_income);


	meta_data = income + "|" + std::to_string(cost);
	if (meta_data.length() <= buffer.length())
	{
		while (meta_data.length() < buffer.length())
		{
			meta_data += ' ';
		}

		wFile.seekp(0, std::ios::beg);
		wFile << meta_data;
		wFile.close();
		rFile.close();
	}
	else
	{
		while (!rFile.eof())
		{
			std::getline(rFile, buffer);
			std::cout << buffer << std::endl;
			record_list.push_back(buffer);
		}
		wFile.close();

		std::ofstream newFile(location, std::ios::out);
		newFile << meta_data << std::endl;
		for (int i = 0; i < record_list.size(); i++)
		{
			newFile << record_list[i] << std::endl;
		}
		newFile.close();
		rFile.close();
	}
}	

/*
	Parameter
		ī�װ� �̸��� �˻��� ������ ��¥, ������ ��¥�� ����, ���� �׸��� �޴´�.
*/
const int __Get_total_from_to(std::string category_name, std::string start_date, std::string end_date, bool flag)
{
	std::string start_month = start_date.substr(0, 7);
	std::string end_month = end_date.substr(0, 7);
	std::string location = CATEGORY + category_name + "/" + start_month + ".txt";
	std::string month_point = start_month;
	int total = 0;

	while (true)
	{
		try
		{
			month_point = __Get_nearest_mon_exist_file(category_name, month_point, end_month);
		}
		catch (int)
		{
			break;
		}

		location = CATEGORY + category_name + "/" + month_point + ".txt";
		std::ifstream rFile(location, std::ios::in);
		rFile.seekg(0, std::ios::beg);

		std::string date;
		std::string buffer;
		std::string meta_buffer;
		std::getline(rFile, meta_buffer);

		while (!rFile.eof())
		{
			std::getline(rFile,buffer);

			try
			{
				date = __Return_time_from_record(buffer).substr(0, 10);
			}
			catch (bool)
			{
				break;
			}
			
			if (__Compare_date(date, start_date) != 1 && __Compare_date(date, end_date) != 2)
			{
				if (flag == INCOME)
				{
					if (buffer.find('+') == buffer.npos)
					{
						continue;
					}
					else
					{
						total += std::stoi(buffer.substr(buffer.find('+') + 1));
					}
				}
				else
				{
					if (buffer.find('+') != buffer.npos)
					{
						continue;
					}
					else
					{
						total += std::stoi(buffer.substr(buffer.find("|-") + 2));
					}
				}//end of income,expense if
			}
			else if (__Compare_date(date, start_date) == 1)//������ ���ڵ尡 ����
			{
				continue;
			}
			else if (__Compare_date(date, end_date) == 2)//������ ���ڵ尡 �̷�
			{
				break;
			}
		}//end of line while
		month_point = __Next_mon_string(month_point);
		rFile.close();
	}//end of file while

	return total;
}

/*
	databuffer�� start_index���� cnt�� ��ŭ ����ش�.
	0 : ����
	-1 : ���� �ε��� ���� �����÷ο�
	-2 : cnt ���� �����÷ο�
*/
const int __Get_data(std::string* data_buffer, std::string category_name, std::string which_month, int start_index, int cnt)
{
	std::string location = CATEGORY + category_name + "/" + which_month + ".txt";
	std::ifstream rFile(location, std::ios::in);
	rFile.seekg(0, std::ios::beg);

	std::string metadata;
	std::getline(rFile, metadata);

	for (int i = 0; i < start_index-1; i++)
	{
		if (rFile.eof() == true) { return-1; }
		std::string buffer;
		std::getline(rFile, buffer);
	}

	for (int i = 0; i < cnt; i++)
	{
		if (rFile.eof() == true) { return -2; }
		std::getline(rFile, data_buffer[i]);
	}
	return 0;
}

void __Get_day_record(std::vector<std::string>& day_record ,std::string date)
{
	std::string location = MASTER;
	std::string categories[MAX_CATEGORY_NUMBER];
	int cnt = __Get_all_file(categories, location);
	std::string* file_location = new std::string[cnt];

	for (int i = 0; i < cnt; i++)
	{
		file_location[i] = CATEGORY + categories[i].substr(0, categories[i].find(".txt")) + "/" + date.substr(0, 7) + ".txt";
		std::ifstream rFile(file_location[i], std::ios::in);
		std::string metadata;
		std::getline(rFile, metadata);

		while (rFile.eof() != true)
		{
			std::string record;
			std::string record_date;
			std::getline(rFile, record);
			record_date = __Return_time_from_record(record).substr(0, 10);
			if (record_date == "0")
				break;
			if (__Compare_date(record_date, date) == 0)
			{
				day_record.push_back(record);
			}
			else if (__Compare_date(record_date, date) == 1)
			{
				continue;
			}
			else if (__Compare_date(record_date, date) == 2)
			{
				break;
			}
		}//end of while
		rFile.close();
	}



	
	delete[] file_location;
}