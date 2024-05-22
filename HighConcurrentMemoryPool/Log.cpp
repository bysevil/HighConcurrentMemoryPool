#include "Log.h"

Log::Log(std::string logFilePath ="./Log/log.txt")
	:_logFilePath(logFilePath)
{
	_ostrm = new std::ofstream(_logFilePath, std::ofstream::app);
}


Log* Log::CreateLog(std::string logFilePath)
{
	if (_log == nullptr) {
		Log* log = new Log(logFilePath);
	}
	return _log;
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

void Log::GC::DestructionLog()
{
	if (ProjectLog) {
		delete ProjectLog;
	}
}
