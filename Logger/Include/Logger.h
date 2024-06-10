#pragma once
#ifdef _WINDLL
#define LoggerDLL_API __declspec(dllexport)
#else
#define LoggerDLL_API __declspec(dllimport)
#endif

#include <string>
using namespace std;

enum class LoggerDLL_API eLoggingOpt {
	ALL_PRINT = 0,
	ONLY_FILE,
	ONLY_CONSOLE
};

class LoggerDLL_API Logger
{
public:
	static Logger& GetInstance() {
		static Logger instance;
		return instance;
	}
	void write_Log(eLoggingOpt loggingOption, string functionName, string log);
	void write_Log_NoEndl(eLoggingOpt loggingOption, string functionName, string log);
private:
	tm* now_time;

	Logger() = default;

	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;
	Logger(Logger&&) = delete;
	Logger& operator=(Logger&&) = delete;

	string getCurrentTimetoString();
	string setPaddingString(std::string const& str);

	void start_Logging(eLoggingOpt loggingOption, std::string& log);

	void printConsole(const string& log);
	void writeLogFile(const string& log);

	void check_LogDir(std::string& logDir);

	void check_Filesize(std::string& logPath, std::string& logDir);

	void press_PrevUntillLog(std::string& logDir);
	void pressLogFile(string prevFilename);

	void make_UntillLog(std::string& logDir, std::string& logPath);
	
	

	
};