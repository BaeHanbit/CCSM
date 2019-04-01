#include "file.h"
#include "function.h"


/*
	해당 카테고리가 존재하는지 확인합니다.
*/
const bool __Find_dir(std::string name)
{
	return access(name.c_str(), 00) == 0 ? 1 : 0;
}
/*
	인자로 받은 이름의 디렉토리를 만듭니다.
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
	Category 폴더 밑에 자신만의 폴더 생성
	자신만의 폴더 밑에 만든 달의 파일 생성
	Master 폴더 아래 자신의 메타 데이터를 저장할 파일 생성
*/
void __Create_category(std::string name, int reset_date)
{
	__Create_dir(MASTER);
	__Create_dir(CATEGORY + name);

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	std::string file_name = CATEGORY + name + "/" + std::to_string(tm.tm_year + 1900) + '-' + __make_perfect_month(tm.tm_mon + 1);

	//Category month file 양식 : Total Income | Total Expense | Balace
	__Create_txt_file(file_name);
	__Create_txt_file(MASTER + name);

	file_name += ".txt";
	std::ofstream wFile(file_name, std::ios::in | std::ios::out);
	wFile.seekp(0, std::ios::beg);
	std::string content = "0|0|0";
	wFile << content << std::endl;

	std::string master_file = MASTER + name + ".txt";
	std::ofstream wFile2(master_file);

	//Master File 양식 : Reset_date | Date when Category was made
	content = __make_perfect_day(reset_date) + '|' + __make_perfect_date(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
	wFile2 << content << std::endl;
}
/*
	인자로 들어온 경로와 이름의 파일 생성
*/
void __Create_txt_file(std::string name)
{
	std::string file_name = name + ".txt";
	std::ofstream file(file_name);
}
/*
	해당 카테고리 폴더를 지웁니다.
	이때 Master에 자기 자신이 파일이 존재해야 합니다.
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
	Master폴더 밑에 존재하는 자신의 정보가 들어있는 파일 삭제
*/
void __Remove_category_file(std::string name)
{
	std::string location = MASTER + name + ".txt";
	std::remove(location.c_str());
}
/*
	입력 받은 버퍼에, 해당 경로(디렉토리)안의 모든 txt파일 정보를 담아줌
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

void __Insert_cate_data(std::string category_name, std::string time, std::string memo, int cost, bool type)
{
	std::string today = __today_date();
	today = today[0] + today[1] + today[2] + today[3] + today[4] + today[5] + today[6];
	std::string location = CATEGORY + category_name + "/" + today;

	__Create_txt_file(location);
	location += ".txt";
	std::ofstream wFile(location, std::ios::in | std::ios::out);
	int index = __Get_index();
	__Set_index(++index);
}

void __Create_index_file()
{
	__Create_txt_file("./Index");
	std::ofstream wFile("./Index.txt", std::ios::in || std::ios::out);
	wFile << 0;
}
int __Get_index()
{
	char buffer[100];
	std::ifstream rFile("./Index.txt", std::ios::in | std::ios::out);
	rFile.getline(buffer, 100);
	return std::stoi(buffer);
}
void __Set_index(int index)
{
	std::ofstream wFile("./Index.txt", std::ios::in | std::ios::out);
	wFile.seekp(0, std::ios::beg);
	wFile << index;
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
		cate : 카테고리 이름
		text : 메모
		time : 시간
		date : 날짜
		type : 수입 지출
		index : 카테고리와 연동되는 고유 인덱스
*/
void __add_record_day(std::string cate, int  cost, std::string text, std::string date, bool type, int index)
{
	std::string location = "./Day/" + date + ".txt";
	std::ofstream wFile(location, std::ios::in | std::ios::out);
	wFile.seekp(0, std::ios::end);

	char symbol = type == INCOME ? '+' : '-';
	std::string content = std::to_string(index) + '|' + symbol;
	content += std::to_string(cost) + '|' + __current_time() + '|' + text + '|' + cate;
	
	wFile << content << std::endl;
}




/*
	Return
		카테고리 개수 
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
		temp : 입력 받은 string 배열에 이름들을 넣어준다.
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
		1. 실제 카테고리 파일 삭제
		2. MasterCategory 파일에서 해당 카테고리 이름 부분 삭제
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

const int __get_cate_total_datas(std::string cate, std::string start_date, std::string end_date, bool type)
{
	return 0;
}

/*
	Parameter
		name : 카테고리 이름
		cost : 금액
		text : 메모
		time : 시간
		type : 수입, 지출 (0 : 수입 | 1 : 지출)
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

	rFile.seekg(0, std::ios::beg);
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
	wFile_master << index;
	wFile_master.close();
}

/*
	Parameter
		buffer : 데이터 저장 공간
		cate_name : 카테고리 이름
		cnt 가져올 데이터 행 개수
		start_point : 데이터를 읽기 시작할 위치
*/
void __get_cate_data(std::string* buffer, std::string cate_name, int cnt, int start_point)
{
	std::string location = "./Category/" + cate_name + ".txt";
	std::ifstream rFile(location, std::ios::in);
	std::string temp_buffer;

	rFile.seekg(0, std::ios::beg);
	for (int i = 0; i < start_point; i++)
	{
		std::getline(rFile, temp_buffer);
	}
	for (int i = 0; i < cnt; i++)
	{
		if (rFile.eof())
		{
			break;
		}
			
		std::getline(rFile, buffer[i]);
	}
}

/*
	Parameter
		cate : 삭제할 기록이 포함된 카테고리
		index 삭제할 기록의 고유 넘버
*/
void __remove_record_cate(std::string cate, int index)
{
	std::string location = "./Category/" + cate + ".txt";
	FILE *fp = fopen(location.c_str(), "a+");
	std::vector<std::string> record_buffer;
	char buffer[100];
	int file_size = 0;
	int flag = false;


	while (!feof(fp))
	{
		int temp_size = ftell(fp);
		fgets(buffer, 50, fp);
		if (flag)
		{
			record_buffer.push_back(std::string(buffer));
		}
		else if (__get_num_from_string(buffer) == index)
		{
			file_size = temp_size;
			flag = true;
		}
	}

	chsize(fileno(fp), file_size);
	fseek(fp, 0, SEEK_END);
	
	for (int i = 0; i < record_buffer.size(); i++)
	{
		fputs(record_buffer[i].c_str(), fp);
	}

	fclose(fp);
}
//-----------------------------------------------------------------------------------//

/*
	Parameter
		name : 카테고리 이름
		flag : 리셋 여부의 flag{0: 초기화 X | 1: 초기화}
*/
const bool __set_flag(std::string name, bool flag)
{			
	std::string location = "./Category/" + name + ".txt";
	//또한, ios::app 모드로 파일을 열었을 경우 seekp() 함수가 먹히지 않습니다.
	//당장은 이동을 하지만, 출력을 할 경우 다시 파일 끝에서부터 출력합니다.
	std::ofstream file(location,std::ios::in | std::ios::out);
	
	file.seekp(25, std::ios::beg);
	file << flag;

	return flag;
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