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
		string a_word = "";  //�ϲ��ɵĵ���
		bool last_is_alpha = false;  //��һ���ַ��Ƿ�����ĸ 
		bool last_is_else = false;  //��һ�������Ƿ��� else

		while ((c = fgetc(fp)) != EOF) {
			if (isalpha(c)) {  //���ַ���ƴ�ӳɵ���
				a_word += c;
				last_is_alpha = true;
			}
			else {
				if (last_is_alpha) {  //һ�����ʽ��� 
					if (key[a_word]) {  //�õ����ǹؼ��� 
						keyword_in_file.push_back(a_word);  //�����������йؼ��֣������ؼ������顱
						if (a_word == "else") {
							if_and_else_in_file.push_back("else");  //�������� if��else��else if �ͻ����ţ�����if else���顱
							if (c != ' ' && c != '\n') {  //else ��������ǿո�����򲻿����� else if �ṹ
								last_is_else = false;  //��ʾ�� else �������� else if �ṹ
							}
							else {
								last_is_else = true;
							}
						}
						if (a_word == "if") {  //���� if Ҫ�ж��� else if ���ǵ�һ��if
							if (last_is_else) {  //��һ���� else�������� else if
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
				else {  //else �������ǿո�ͻ���Ҫ�ı���
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
