#pragma once
#ifdef _WINDLL
#define LoggerDLL_API __declspec(dllexport)
#else
#define LoggerDLL_API __declspec(dllimport)
#endif

#include <string>
using namespace std;

class LoggerDLL_API Logger
{
public:
	static Logger& GetInstance() {
		static Logger instance;
		return instance;
	}
	void write_Log(string log);
private:
	Logger() = default;

	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;
	Logger(Logger&&) = delete;
	Logger& operator=(Logger&&) = delete;
};