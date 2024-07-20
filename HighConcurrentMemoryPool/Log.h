#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <ctime>
#include <thread>

constexpr auto MAX_LOG_FILE_SIZE = 1e10; //������־�ļ�����С
constexpr auto MAX_LOG_FILE_NUM = 10; //����ж��ٸ���־�ļ�

//��ȡ��ǰ���ں�ʱ����ַ�����ʾ
std::string get_nowTime();

//��־��
class Log{
public:
	//��־����
	enum level{
		INFO,	//��ͨ
		DEBUG,	//����
		WARN,	//����
		LOG_ERROR,	//����
		FATAL	//����
	};

	//ʹ�õ���ģʽ����������ʼ��
	static Log* CreateLog(std::string logFilePath = "./Log/Log", level outFileLV= Log::INFO, level outCmdLV = Log::LOG_ERROR);

	//��¼��־
	static void record(level ,std::string logText);

	// ��ȡ��־�ļ���·��
	static std::string getLogFileName();

	// ��ȡ��־�ļ���
	static std::string getLogFilePath();

	//��ȡ��־����
	static std::string getLevel();

	//���ٵ�����־��
	static void DestructionLog();

	//�������������ļ�����־����
	static void setOutFileLV(level outFileLV);

	//����������������̨����־����
	static void setOutCmdLV(level outCmdLV);
private:
	//��־����ʱ������һ����־�ļ�
	void switchLogFile();

	
	//�Զ�����
	struct GC {
		~GC();
	};

	Log(std::string logFilePath,level outFileLV, level outCmdLV);
	~Log();

	Log(const Log& obj) = delete;
	Log& operator=(const Log& obj) = delete;

	static Log* _log; // ��������
	static GC _gc; 
	std::string _logFileName; //��־�ļ���(��·��)
	std::string _logFilePath; //��־�ļ���
	std::ofstream* _ostrm; //�򿪵���־�ļ���
	int _filenum; //��ǰʹ�õ���־�ļ����
	static std::mutex _mtx; 
	level _lv; // ��־����
	level _outFileLV; //��������ļ��������־����
	level _outCmdLV; //�����������̨�������־����
};


