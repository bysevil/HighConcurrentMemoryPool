#include "Log.h"

Log::Log(std::string logFilePath ="./Log/log.txt")
	:_logFilePath(logFilePath)
{
	_ostrm = new std::ofstream(_logFilePath, std::ofstream::app);
}

void Log::record(std::string logText)
{
	std::time(&timenum);
	// 转换为本地时间并输出
	ctime_s(timestr, sizeof(timestr), &timenum);

	logText = timestr + logText;
	*_ostrm << logText.c_str() << std::endl;
}

std::string Log::getfilename()
{
	return _logFilePath;
}
