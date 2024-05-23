#pragma once
#include<iostream>
#include<fstream>
#include <string>
#include <mutex>
#include <ctime>

//日志类
//最多有10个日志文件
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

	//日志过大时删除日志
private:
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
	static std::mutex _mtx; 
};


