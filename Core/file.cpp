#include "file.h"
#include "function.h"

/*
	Parameter
		name : 만들 디렉토리 이름

	Return
		-1 : 같은 디렉토리 있음, 실패
		0 : 성공
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
		name : 디렉토리 이름

	Return
		0 : 디렉토리 없음
		1 : 디렉토리 존재
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
		year : 연도
		mon : 달
		day : 일

	Return
		0 : 파일 없음
		1 : 파일 존재
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
		name : 카테고리 이름

	Return
		-1 : 같은 카테고리 존재 실패
		 0 : 성공
*/
const int __create_cate(std::string name, int reset_day)
{
	if (__find_cate(name))
	{
		return -1;
	}
	else
	{
		//reset | last | next | flag(0 : 초기화 필요X | 1 : 초기화 필요)
		std::string file_name = "Category/" + name + ".txt";
		std::ofstream file(file_name);
		file << reset_day << '|' << __today_date() << '|' << __next_reset_date(reset_day) << '|' << 0;
		return 0;
	}
}

/*
	Parameter
		name : 카테고리 이름

	Return
		0 : 파일 존재 하지 않음
		1 : 파일 존재
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
		file_name : 검사 파일 이름

	Return
		0 : 존재하지 않음
		1 : 존재
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