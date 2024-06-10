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
private:
	Logger() = default;

	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;
	Logger(Logger&&) = delete;
	Logger& operator=(Logger&&) = delete;

	string getCurrentTimetoString();
	tm* now_time;
	string setPaddingString(std::string const& str);

	void pressLogFile(string prevFilename);


	void printConsole(const string& log);
	void writeLogFile(const string& log);
	void check_Filesize(std::string& logPath, std::string& logDir);
	void make_UntillLog(std::string& logDir, std::string& logPath);
	void press_PrevUntillLog(std::string& logDir);
	void check_LogDir(std::string& logDir);
};