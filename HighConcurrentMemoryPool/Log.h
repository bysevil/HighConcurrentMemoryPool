#pragma once
#include<iostream>
#include<fstream>
#include <string>
#include <mutex>
//��־��
class Log{
public:
	//ʹ�õ���ģʽ����������ʼ��
	static Log* CreateLog(std::string logFilePath);

	//��¼��־
	void record(std::string logText);

	// ��ȡ��־�ļ�·��
	std::string getfilename();

	

private:
	//���ٵ�����־��
	class GC {
	public:
		void DestructionLog();
	};
	//��ʼ��ʱ������������ļ�Ŀ¼
	Log(std::string logFilePath);

	static Log* _log;
	static GC _gc;
	std::string _logFilePath; //��־�ļ�·��
	std::ofstream* _ostrm; //�򿪵���־�ļ���
	static std::mutex _mtx;
};

Log* Log::_log = CreateLog("./");
std::mutex Log::_mtx;
Log::GC Log::_gc;

static std::time_t timenum;

static char timestr[26];