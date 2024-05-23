#pragma once
#include<iostream>
#include<fstream>
#include <string>
#include <mutex>
#include <ctime>

//��־��
//�����10����־�ļ�
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

	//��־����ʱɾ����־
private:
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
	static std::mutex _mtx; 
};


