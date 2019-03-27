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
		file << reset_day << '|' << __today_date() << '|' << __next_reset_date(reset_day) << '|' << 0;
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