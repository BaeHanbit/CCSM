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
	return cnt-1;
}

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
		file << __make_perfect_day(reset_day) << '|' << __today_date() << '|' << __next_reset_date(reset_day) << '|' << flag << '|' << index << std::endl;
		file.close();

		std::ofstream master_file("MasterCategory.txt",std::ios::in|std::ios::out);
		master_file.seekp(0, std::ios::end);
		master_file << name << std::endl;
		master_file.close();
		return 0;
	}
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
	std::ofstream wFile(location, std::ios::in|std::ios::out);
	std::ifstream rFile(location, std::ios::in);

	std::string temp;
	std::string max_index_temp;
	std::getline(rFile, temp);

	for (int i = 27; i < temp.length(); i++)
	{
		max_index_temp += temp[i];
	}
	int index = std::stoi(max_index_temp) + 1;

	wFile.seekp(27, std::ios::beg);
	wFile << index;

	char symbol = type == INCOME ? '+' : '-';

	std::string().swap(temp);
	temp = std::to_string(index) + '|' + symbol;
	temp += std::to_string(cost) + '|' + time + '|' + text;

	wFile.seekp(0, std::ios::end);
	wFile << temp << std::endl;
}

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