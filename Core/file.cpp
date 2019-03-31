#include "file.h"
#include "function.h"

/*
	Parameter
		name : ���� ���丮 �̸�

	Return
		-1 : ���� ���丮 ����, ����
		0 : ����
*/
const int __create_dir(std::string name)
{
	if (__find_dir(name))
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
	Parameter
		name : ���丮 �̸�

	Return
		0 : ���丮 ����
		1 : ���丮 ����
*/
const bool __find_dir(std::string name)
{
	return access(name.c_str(), 00) == 0 ? 1 : 0;
}

//-----------------------------------------------------------------------------------//

/*
	[ Return Value ]
		{-1} : failed to create day : same day have already exist
		{0} : Success to create day

	[ Function Act ]
		Create day.
*/
const int __create_day(int year, int mon, int day)
{
	if (__find_day(year, mon, day))
	{
		return -1;
	}
	else
	{
		std::string date = __make_perfect_date(year, mon, day);
		std::string file_name = "./Day/" + date + ".txt";
		std::ofstream file(file_name);
		return 0;
	}
}

/*
	Parameter
		year : ����
		mon : ��
		day : ��

	Return
		0 : ���� ����
		1 : ���� ����
*/
const bool __find_day(int year, int mon, int day)
{
	std::ifstream file;
	std::string date = __make_perfect_date(year, mon, day);
	std::string file_name = "./Day/" + date + ".txt";

	if (__check_file_exist(file_name))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//-----------------------------------------------------------------------------------//

/*
	Return
		-1 : ���� : ���� ����
		0 : ����
*/
const int __create_master_cate()
{
	std::string location = "./MasterCategory.txt";
	if (__check_file_exist(location))
	{
		return -1;
	}
	else
	{
		std::ofstream wFile(location);
		wFile << 0 << std::endl;
		return 0;
	}
}

/*
	Return
		ī�װ� ���� 
*/
const int __cate_cnt()
{
	std::string location = "MasterCategory.txt";
	std::ifstream master_cate(location,std::ios::in | std::ios::out);
	char temp[100] = { 0 };

	int cnt = 0;
	while (!master_cate.eof())
	{
		master_cate.getline(temp, 100);
		cnt++;
	}
	return cnt-2;
}

/*
	Parameter
		temp : �Է� ���� string �迭�� �̸����� �־��ش�.
*/
void __get_cate_names(std::string* temp)
{
	std::string location = "MasterCategory.txt";
	std::ifstream file(location, std::ios::in | std::ios::out);
	char temp_c_str[42] = { 0 };

	file.seekg(0, std::ios::beg);
	
	for (int i = 0; i < __cate_cnt(); i++)
	{
		file.getline(temp_c_str, 41);
		std::string _temp(temp_c_str);
		temp[i] = _temp;
	}
}

/*
	Parameter
		name : ī�װ� �̸�

	Return
		-1 : ���� ī�װ� ���� ����
		 0 : ����
*/
const int __create_cate(std::string name, int reset_day)
{
	if (__find_cate(name))
	{
		return -1;
	}
	else
	{
		//reset | last | next | flag(0 : �ʱ�ȭ �ʿ�X | 1 : �ʱ�ȭ �ʿ�)
		std::string file_name = "Category/" + name + ".txt";
		std::ofstream file(file_name);
		int flag = 0;
		int index = 0;
		file << __make_perfect_day(reset_day) << '|' << __today_date() << '|' << __next_reset_date(reset_day) << '|' << flag << std::endl;
		file.close();

		std::ofstream master_file("MasterCategory.txt",std::ios::in|std::ios::out);
		master_file.seekp(0, std::ios::end);
		master_file << name << std::endl;
		master_file.close();
		return 0;
	}
}

/*
	Function Act
		1. ���� ī�װ� ���� ����
		2. MasterCategory ���Ͽ��� �ش� ī�װ� �̸� �κ� ����
*/
void __remove_cate(std::string name)
{
	std::string location = "./Category/" + name + ".txt";
	std::remove(location.c_str());

	location = "./MasterCategory.txt";

	FILE *fp = fopen(location.c_str(), "a+");

	std::list<std::string> cate_list;
	std::list<std::string>::iterator list_iter;

	int index_where = -1;
	int file_size = 0;
	char temp_for_index[20];
	
	rewind(fp);
	fgets(temp_for_index, 20, fp);

	for (int i = 0; i < __cate_cnt(); i++)
	{
		int temp_file_size = ftell(fp);
		char temp_buffer[30];
		fgets(temp_buffer, 30, fp);
		std::string buffer(temp_buffer);

		if (index_where != -1)
		{
			cate_list.push_back(buffer);
		}

		if (buffer == name + "\n")
		{
			file_size = temp_file_size;
			index_where = i;
		}
	}

	chsize(fileno(fp),file_size);

	fseek(fp, 0, SEEK_END);

	for (list_iter = cate_list.begin(); list_iter != cate_list.end(); list_iter++)
	{
		fputs(list_iter->c_str(), fp);
	}
	fclose(fp);
}

/*
	Parameter
		name : ī�װ� �̸�

	Return
		0 : ���� ���� ���� ����
		1 : ���� ����
*/
const bool __find_cate(std::string name)
{
	std::string file_name = "Category/" + name + ".txt";
	if (__check_file_exist(file_name))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
	Parameter
		name : ī�װ� �̸�
		cost : �ݾ�
		text : �޸�
		time : �ð�
		type : ����, ���� (0 : ���� | 1 : ����)
*/
void __add_record_cate(std::string name, int cost, std::string text, std::string time, bool type)
{
	std::string location = "./Category/" + name + ".txt";
	std::string master_location = "./MasterCategory.txt";
	std::ofstream wFile(location, std::ios::in|std::ios::out);
	std::ifstream rFile(location, std::ios::in);
	std::ofstream wFile_master(master_location, std::ios::in | std::ios::out);
	std::ifstream rFile_master(master_location, std::ios::in);
	
	std::string index;
	std::getline(rFile_master, index);
	char symbol = type == INCOME ? '+' : '-';

	std::string content;
	content = index + '|' + symbol;
	content += std::to_string(cost) + '|' + time + '|' + text;

	wFile.seekp(0, std::ios::end);
	wFile << content << std::endl;
	wFile.close();

	int temp_index = std::stoi(index) + 1;
	index = std::to_string(temp_index);
	wFile_master.seekp(0, std::ios::beg);
	wFile_master << index << std::endl;
	wFile_master.close();
}

//-----------------------------------------------------------------------------------//

/*
	Parameter
		name : ī�װ� �̸�
		flag : ���� ������ flag{0: �ʱ�ȭ X | 1: �ʱ�ȭ}
*/
const bool __set_flag(std::string name, bool flag)
{			
	std::string location = "./Category/" + name + ".txt";
	//����, ios::app ���� ������ ������ ��� seekp() �Լ��� ������ �ʽ��ϴ�.
	//������ �̵��� ������, ����� �� ��� �ٽ� ���� ���������� ����մϴ�.
	std::ofstream file(location,std::ios::in | std::ios::out);
	
	file.seekp(25, std::ios::beg);
	file << flag;

	return flag;
}

/*
	Parameter
		file_name : �˻� ���� �̸�

	Return
		0 : �������� ����
		1 : ����
*/
const bool __check_file_exist(std::string file_name)
{
	std::ifstream file(file_name);
	if (file.good())
	{
		file.close();
		return true;
	}
	else
	{
		file.close();
		return false;
	}
}