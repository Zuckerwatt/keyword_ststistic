#pragma warning(disable : 4996)
#include<sys/types.h>
#include<sys/stat.h>
#include<iostream>
#include<vector>
#include<map>
#include<stdio.h>
#include<stack>
using namespace std;

map <string,int> key; 

int total_cnt=0; 
int switch_cnt = 0; 
vector <int> case_cnt; 
int if_else_cnt = 0;
int if_elseif_else_cnt = 0;
int else_if_cnt = 0;
int else_cnt = 0;

vector <string> keyword_in_file;  
vector <string> if_and_else_in_file; 

char file_path[100];
FILE * fp;

void InitKey(); 

void Input();

void OpenAndReadFileToWord(); 

void Statistic(); 
void Output();  





void InitKey()   
{
	key["auto"] = key["double"] = key["int"] = key["struct"]
		= key["break"] = key["else"] = key["long"] = key["switch"]
		= key["case"] = key["enum"] = key["register"] = key["typedef"]
		= key["char"] = key["extern"] = key["return"] = key["union"]
		= key["const"] = key["float"] = key["short"] = key["unsigned"]
		= key["continue"] = key["for"] = key["signed"] = key["void"]
		= key["default"] = key["goto"] = key["sizeof"] = key["volatile"]
		= key["do"] = key["if"] = key["static"] = key["while"] = 1;
}

void Input()
{
	cout << "Please enter the file path: ";
	cin >> file_path;
}

void OpenAndReadFileToWord() 
{
	fp = fopen( file_path, "r");
	if (fp == NULL)
	{
		printf("falt to open the file : %s.\n", file_path);
		return;
	}
	else {
		
		char c;  
		string a_word = "";  //合并成的单词
		bool last_is_alpha = false;  //上一个字符是否是字母 
		bool last_is_else = false;  //上一个单词是否是 else

		while ((c = fgetc(fp)) != EOF) {
			if (isalpha(c)) {  //是字符则拼接成单词
				a_word += c;
				last_is_alpha = true;
			}
			else {
				if (last_is_alpha) {  //一个单词结束 
					if (key[a_word]) {  //该单词是关键字 
						keyword_in_file.push_back(a_word);  //该数组存放所有关键字，即“关键字数组”
						if (a_word == "else") {
							if_and_else_in_file.push_back("else");  //该数组存放 if、else、else if 和花括号，即“if else数组”
							if (c != ' ' && c != '\n') {  //else 后如果不是空格或换行则不可能是 else if 结构
								last_is_else = false;  //表示该 else 不可能是 else if 结构
							}
							else {
								last_is_else = true;
							}
						}
						if (a_word == "if") {  //遇到 if 要判断是 else if 还是单一个if
							if (last_is_else) {  //上一个是 else，所以是 else if
								last_is_else = false;
								if_and_else_in_file[if_and_else_in_file.size() - 1] += "_if";
								else_if_cnt++;
							}
							else {
								if_and_else_in_file.push_back("if");
							}
						}
					}
					a_word = "";
				}
				else {  //else 后碰到非空格和换行要改变标记
					if (c != ' ' && c != '\n') {
						last_is_else = false;
					}
				}
				last_is_alpha = false;
				if (c == '{' || c == '}') {
					string temp = "";
					temp += c;
					if_and_else_in_file.push_back(temp);
				}
			}
		}
		if (key[a_word]) { 
			keyword_in_file.push_back(a_word);
		}
		fclose(fp);
	}
}



void Statistic()  
{

	for (int i = 0;i < keyword_in_file.size();i++) {
		total_cnt++;
	}

	bool have_switch = false;
	int case_num = 0;
	for (int i = 0;i < keyword_in_file.size();i++) {
		if (keyword_in_file[i] == "switch") {
			if (have_switch) { 
				case_cnt.push_back(case_num);
				case_num = 0;
			}
			else {
				have_switch = true;
			}
			switch_cnt++;
		}
		if (keyword_in_file[i] == "case") {
			case_num++;
		}
	}
	case_cnt.push_back(case_num);

	stack <string> s;  
	stack <int> have_elseif; 

	for (int i = 0;i < if_and_else_in_file.size();i++) {
		if (if_and_else_in_file[i] == "{") {
			s.push("{");
			have_elseif.push(0);
		}
		else if (if_and_else_in_file[i] == "}") { 
			while (s.top() != "{") {
				s.pop();
			}
			s.pop();
			have_elseif.pop();
		}
		else if (if_and_else_in_file[i] == "if") {
			s.push("if");
			have_elseif.pop();
			have_elseif.push(0);
		}
		else if (if_and_else_in_file[i] == "else_if") {
			have_elseif.pop();
			have_elseif.push(1);
		}
		else if (if_and_else_in_file[i] == "else") { 
			if (have_elseif.top()) {
				if_elseif_else_cnt++;
			}
			else {
				if_else_cnt++;
			}
			have_elseif.pop();
			have_elseif.push(0);
			while (s.top() != "if") {
				s.pop();
			}
			s.pop();
		}
	}
}




void Output()
{
	cout << "total num: " << total_cnt << endl;
	cout << "switch num: " << switch_cnt << endl;
	cout << "case num:";
	for (int i = 0;i < case_cnt.size();i++) {
		cout << " " << case_cnt[i];
	}
	cout << endl;
	cout << "if-else num: " << if_else_cnt << endl;
	cout << "if-elseif-else num: " << if_elseif_else_cnt << endl;
}

int main() 
{
	InitKey();
	Input();
	OpenAndReadFileToWord();
    Statistic();
	Output();
	return 0;
}
