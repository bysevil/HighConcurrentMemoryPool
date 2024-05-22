#pragma once
#include<iostream>
#include<fstream>
#include <string>

//使用单例模式（懒汉）
class Log{
public:
	//初始化时传入错误日期文件目录
	Log(std::string logFilePath);

	//记录日志
	void record(std::string logText);

	// 获取日志文件路径
	std::string getfilename();

private:
	std::string _logFilePath;
	std::ofstream* _ostrm;
};

static Log* ProjectLog = new Log("./Log.txt");

static std::time_t timenum;

static char timestr[26];