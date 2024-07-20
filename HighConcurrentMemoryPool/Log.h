#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <ctime>
#include <thread>

constexpr auto MAX_LOG_FILE_SIZE = 1e10; //单个日志文件最大大小
constexpr auto MAX_LOG_FILE_NUM = 10; //最多有多少个日志文件

//获取当前日期和时间的字符串表示
std::string get_nowTime();

//日志类
class Log{
public:
	//日志级别
	enum level{
		INFO,	//普通
		DEBUG,	//调试
		WARN,	//警告
		LOG_ERROR,	//错误
		FATAL	//致命
	};

	//使用单例模式（饿汉）初始化
	static Log* CreateLog(std::string logFilePath = "./Log/Log", level outFileLV= Log::INFO, level outCmdLV = Log::LOG_ERROR);

	//记录日志
	static void record(level ,std::string logText);

	// 获取日志文件夹路径
	static std::string getLogFileName();

	// 获取日志文件名
	static std::string getLogFilePath();

	//获取日志级别
	static std::string getLevel();

	//销毁单例日志类
	static void DestructionLog();

	//设置最低输出至文件的日志级别
	static void setOutFileLV(level outFileLV);

	//设置最低输出至控制台的日志级别
	static void setOutCmdLV(level outCmdLV);
private:
	//日志过大时进入下一个日志文件
	void switchLogFile();

	
	//自动销毁
	struct GC {
		~GC();
	};

	Log(std::string logFilePath,level outFileLV, level outCmdLV);
	~Log();

	Log(const Log& obj) = delete;
	Log& operator=(const Log& obj) = delete;

	static Log* _log; // 单例对象
	static GC _gc; 
	std::string _logFileName; //日志文件名(带路径)
	std::string _logFilePath; //日志文件夹
	std::ofstream* _ostrm; //打开的日志文件流
	int _filenum; //当前使用的日志文件编号
	static std::mutex _mtx; 
	level _lv; // 日志级别
	level _outFileLV; //会输出至文件的最低日志级别
	level _outCmdLV; //会输出到控制台的最低日志级别
};


