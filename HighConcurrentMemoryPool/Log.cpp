#include "Log.h"

Log* Log::_log = CreateLog();
std::mutex Log::_mtx;
Log::GC Log::_gc;

std::string get_nowTime() {
	std::time_t now;
	std::time(&now);
	// 转换为本地时间并输出
	char buffer[50];
	ctime_s(buffer, sizeof(buffer), &now);
	std::string ret(buffer);
	ret.pop_back();
	return ret;
}

Log::Log(std::string logFilePath,level outFileLV,level outCmdLV)
	:_filenum(-1)
	,_outCmdLV(outCmdLV)
	,_outFileLV(outFileLV)
	,_logFilePath(logFilePath)
	,_ostrm(new std::ofstream())
{
	//进入第一个日志文件
	switchLogFile();
}

Log::~Log()
{
	_ostrm->close();
}


Log* Log::CreateLog(std::string logFilePath,level outFileLV,level outCmdLV)
{
	if (_log == nullptr) {
		_mtx.lock();
		if (_log == nullptr) {
			_log = new Log(logFilePath,outFileLV, outCmdLV);
		}
		_mtx.unlock();
	}
	return _log;
}

void Log::record(level lv,std::string logText)
{
	if (_log == nullptr) {
		std::cout <<"_log == nullptr" << std::endl;
		return;
	}
	_log->_lv = lv;
	if (lv >= _log->_outCmdLV) {
		_mtx.lock();
		std::cout << "[thread" << std::this_thread::get_id() << "] ["
			+ get_nowTime() + "] ["
			+ getLevel() + "]:  "
			+ logText.c_str() << std::endl;
		_mtx.unlock();
	}
	if (lv < _log->_outFileLV) {
		return;
	}
	
	_mtx.lock();

	if (_log->_ostrm->tellp() > MAX_LOG_FILE_SIZE) {
		_log->switchLogFile();
	}
	*(_log->_ostrm) << "[thread" << std::this_thread::get_id() << "] ["
		+ get_nowTime() + "] ["
		+ getLevel() + "]:  "
		+ logText.c_str() << std::endl;
	_mtx.unlock();
}

std::string Log::getLogFileName()
{
	return _log->_logFileName;
}

std::string Log::getLogFilePath()
{
	return _log->_logFilePath;
}

std::string Log::getLevel() {
	std::string ret;
	switch (_log->_lv) {
	case INFO:
		ret = "INFO";
		break;
	case DEBUG:
		ret = "DEBUG";
		break;
	case WARN:
		ret = "WARN";
		break;
	case LOG_ERROR:
		ret = "ERROR";
		break;
	case FATAL:
		ret = "FATAL";
		break;
	}
	return ret;
}

void Log::DestructionLog()
{
	if (_log != nullptr) {
		_mtx.lock();
		if (_log != nullptr) {
			delete _log;
		}
		_log = nullptr;
		_mtx.unlock();
	}
}

void Log::setOutFileLV(level outFileLV)
{
	_log->_outFileLV = outFileLV;
}

void Log::setOutCmdLV(level outCmdLV)
{
	_log->_outCmdLV = outCmdLV;
}

void Log::switchLogFile()
{
	_filenum = (_filenum + 1) % MAX_LOG_FILE_NUM;
	_logFileName = _logFilePath + std::to_string(_filenum) + ".txt";
	if (_ostrm->is_open()) {
		_ostrm->close();
	}
	_ostrm->open(_logFileName, std::ofstream::trunc);
	_ostrm->close();
	_ostrm->open(_logFileName, std::ofstream::app);
}

Log::GC::~GC()
{
	DestructionLog();
}
