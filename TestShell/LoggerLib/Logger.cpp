#include "pch.h"
#include "Logger.h"

#include<iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <filesystem>

void Logger::write_Log(eLoggingOpt loggingOption, string sfunctionName, string log_detail) {
	string log = getCurrentTimetoString()+ " " + setPaddingString(sfunctionName+"()") + " : " + log_detail;

	switch (loggingOption)
	{
	case eLoggingOpt::ALL_PRINT:
		printConsole(log);
		printFile(log);
		break;
	case eLoggingOpt::ONLY_FILE:
		printFile(log);
		break;
	case eLoggingOpt::ONLY_CONSOLE:
		printConsole(log);
		break;
	default:
		break;
	}
}


string Logger::getCurrentTimetoString() {
	auto chrono_Now = std::chrono::system_clock::now();
	auto millisec = chrono::duration_cast<chrono::milliseconds>(chrono_Now.time_since_epoch()) % 1000;

	time_t tm_now = chrono::system_clock::to_time_t(chrono_Now);
	tm* now_time = new tm();
	localtime_s(now_time, &tm_now);

	char temp[128];
	strftime(temp, sizeof(temp), "%Y-%m-%d %H:%M:%S.", now_time);
	
	std::ostringstream oss;
	oss << "[" << temp << left << setfill('0') << setw(3) << millisec.count() << "]";

	return oss.str();
}

string Logger::setPaddingString(const string& str)
{
	std::ostringstream oss;
	oss << left << setfill(' ') << setw(30) << str;
	return oss.str();
}

void Logger::printConsole(const string& log) {
	cout << log << endl;
}

void Logger::printFile(const string& log) {

	// dir 존재 여부 체크
	filesystem::path dirPath("log");
	if (filesystem::exists(dirPath) == false) {
		if (create_directory(dirPath)) {
			cout << "Directory created successfully." << endl;
		}
	}

	// 파일 크기 체크 및 변환 구현 필요.

	ofstream logFile("log\\latest.log", ios::app);
	if (logFile.is_open()) {
		logFile << log << endl;
	}

	logFile.close();
}