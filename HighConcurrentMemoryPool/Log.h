#pragma once
#include<iostream>
#include<fstream>
#include <string>

//ʹ�õ���ģʽ��������
class Log{
public:
	//��ʼ��ʱ������������ļ�Ŀ¼
	Log(std::string logFilePath);

	//��¼��־
	void record(std::string logText);

	// ��ȡ��־�ļ�·��
	std::string getfilename();

private:
	std::string _logFilePath;
	std::ofstream* _ostrm;
};

static Log* ProjectLog = new Log("./Log.txt");

static std::time_t timenum;

static char timestr[26];