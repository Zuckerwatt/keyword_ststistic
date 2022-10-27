#pragma warning(disable : 4996)
#include<sys/types.h>
#include<sys/stat.h>
#include<iostream>
#include<vector>
#include<map>
#include<stdio.h>
#include<stack>
using namespace std;

map <string,int> key;  //�ؼ���ö�ٱ�� 

int total_cnt=0;  //�ؼ��ֳ���������¼
int switch_cnt = 0;  //switch ͳ������
vector <int> case_cnt;  //ÿ�� case ��������
int if_else_cnt = 0;
int if_elseif_else_cnt = 0;
int else_if_cnt = 0;
int else_cnt = 0;

vector <string> keyword_in_file;  //���ڴ洢�ļ��г��ֵ����йؼ��� 
vector <string> if_and_else_in_file;  //���ڴ洢�ļ��г��ֵ����� if��else��else-if �Լ�������

char file_path[100];
FILE * fp;

void InitKey();  //��ʼ���ؼ��� 

void Input();

void OpenAndReadFileToWord();  //���ļ����ļ����ݰ���ĸ���������ǹؼ��ֵ���ĸ�ϲ��ɹؼ���

void Statistic(); 
void Output();  



//�����Ǻ���ʵ��

void InitKey()  //��ʼ���ؼ��ֱ�� 
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
}




void Output()
{
	cout << "total num: " << total_cnt << endl;
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
