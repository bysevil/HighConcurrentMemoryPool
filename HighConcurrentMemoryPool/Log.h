#pragma once
#include<iostream>
#include<fstream>
#include <string>
#include <mutex>
#include <ctime>

constexpr auto MAX_LOG_FILE_SIZE = 1e10; //单个日志文件最大大小
constexpr auto MAX_LOG_FILE_NUM = 10; //最多有多少个日志文件
//日志类
class Log{
public:

	//使用单例模式（饿汉）初始化
	static Log* CreateLog();

	//记录日志
	static void record(std::string logText);

	// 获取日志文件路径
	static std::string getfilename();

	//销毁单例日志类
	static void DestructionLog();

	
private:
	//日志过大时进入下一个日志文件
	void switchLogFile();

	//自动销毁
	class GC {
	public:
		~GC();
	};

	Log();
	~Log();

	Log(const Log& obj) = delete;
	Log& operator=(const Log& obj) = delete;

	static Log* _log; // 单例对象
	static GC _gc; 
	std::string _logFilePath; //日志文件路径
	std::ofstream* _ostrm; //打开的日志文件流
	int _filenum; //当前使用的日志文件编号
	static std::mutex _mtx; 
};


