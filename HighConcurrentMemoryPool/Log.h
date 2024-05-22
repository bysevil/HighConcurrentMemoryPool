#pragma once
#include<iostream>
#include<fstream>
#include <string>
#include <mutex>
//日志类
class Log{
public:
	//使用单例模式（饿汉）初始化
	static Log* CreateLog(std::string logFilePath);

	//记录日志
	void record(std::string logText);

	// 获取日志文件路径
	std::string getfilename();

	

private:
	//销毁单例日志类
	class GC {
	public:
		void DestructionLog();
	};
	//初始化时传入错误日期文件目录
	Log(std::string logFilePath);

	static Log* _log;
	static GC _gc;
	std::string _logFilePath; //日志文件路径
	std::ofstream* _ostrm; //打开的日志文件流
	static std::mutex _mtx;
};

Log* Log::_log = CreateLog("./");
std::mutex Log::_mtx;
Log::GC Log::_gc;

static std::time_t timenum;

static char timestr[26];