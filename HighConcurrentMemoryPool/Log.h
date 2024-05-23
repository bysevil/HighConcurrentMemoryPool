#pragma once
#include<iostream>
#include<fstream>
#include <string>
#include <mutex>
#include <ctime>

constexpr auto MAX_LOG_FILE_SIZE = 1e10; //������־�ļ�����С
constexpr auto MAX_LOG_FILE_NUM = 10; //����ж��ٸ���־�ļ�
//��־��
class Log{
public:

	//ʹ�õ���ģʽ����������ʼ��
	static Log* CreateLog();

	//��¼��־
	static void record(std::string logText);

	// ��ȡ��־�ļ�·��
	static std::string getfilename();

	//���ٵ�����־��
	static void DestructionLog();

	
private:
	//��־����ʱ������һ����־�ļ�
	void switchLogFile();

	//�Զ�����
	class GC {
	public:
		~GC();
	};

	Log();
	~Log();

	Log(const Log& obj) = delete;
	Log& operator=(const Log& obj) = delete;

	static Log* _log; // ��������
	static GC _gc; 
	std::string _logFilePath; //��־�ļ�·��
	std::ofstream* _ostrm; //�򿪵���־�ļ���
	int _filenum; //��ǰʹ�õ���־�ļ����
	static std::mutex _mtx; 
};


