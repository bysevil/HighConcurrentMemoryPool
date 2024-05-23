#include "Log.h"

Log* Log::_log = CreateLog();
std::mutex Log::_mtx;
Log::GC Log::_gc;

Log::Log()
	:_filenum(-1)
	,_ostrm(new std::ofstream())
{
	switchLogFile();
}

Log::~Log()
{
	_ostrm->close();
}


Log* Log::CreateLog()
{
	if (_log == nullptr) {
		_mtx.lock();
		if (_log == nullptr) {
			_log = new Log();
		}
		_mtx.unlock();
	}
	return _log;
}

void Log::record(std::string logText)
{
	_mtx.lock();
	std::time_t now;
	std::time(&now);
	// 转换为本地时间并输出
	char buffer[26];
	ctime_s(buffer, sizeof(buffer), &now);

	logText = buffer + logText;
	if (_log->_ostrm->tellp() > MAX_LOG_FILE_SIZE) {
		_log->switchLogFile();
	}
	*(_log->_ostrm) << "thread" << std::this_thread::get_id() << ' ';
	*(_log->_ostrm) << logText.c_str() << std::endl;
	_mtx.unlock();
}

std::string Log::getfilename()
{
	return _log->_logFilePath;
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

void Log::switchLogFile()
{
	_filenum = (_filenum + 1) % MAX_LOG_FILE_NUM;
	_logFilePath = "./Log/Log" + std::to_string(_filenum) + ".txt";
	if (_ostrm->is_open()) {
		_ostrm->close();
	}
	_ostrm->open(_logFilePath, std::ofstream::trunc);
	_ostrm->close();
	_ostrm->open(_logFilePath, std::ofstream::app);
}

Log::GC::~GC()
{
	DestructionLog();
}
