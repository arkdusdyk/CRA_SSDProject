#include "pch.h"
#include "Logger.h"

#include<iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <filesystem>
#include <io.h>

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
	now_time = new tm();
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
	string logDir = "log";
	filesystem::path dirPath(logDir);
	if (filesystem::exists(dirPath) == false) {
		if (create_directory(dirPath)) {
			cout << "Directory created successfully." << endl;
		}
	}

	string logPath = logDir + "\\latest.log";
	ofstream logFile(logPath, ios::app);
	if (logFile.is_open()) {
		logFile << log << endl;
	}
	logFile.close();

	auto fileSize = filesystem::file_size(logPath);
	// 파일 크기 체크 및 변환 구현 필요.
	if (fileSize > 10240)
	{
		string prevFilePath = logDir + "\\untill*.log";
		struct _finddata_t fd;
		intptr_t handle;
		if ((handle = _findfirst(prevFilePath.c_str(), &fd)) == -1L)
		{
			cout << "No file in directory!" << endl;
			// 파일이 없을경우 발생시킬 이벤트.
		}

		do
		{
			string prevFilename = logDir + "\\" + fd.name;
			string chgFilename = prevFilename;
			ChangeExt((char*)chgFilename.c_str(), (char*)".zip");
			filesystem::rename(prevFilename, chgFilename);

		} while (_findnext(handle, &fd) == 0);
		_findclose(handle);


		string newName = logDir;
		char temp[128];
		strftime(temp, sizeof(temp), "%Y%m%d %Hh%Mm%Ss", now_time);
		newName.append("\\untill_");
		newName.append(temp);
		newName.append(".log");
		filesystem::rename(logPath, newName);
		// 이전 until file이 있으면 압축
	}
}


void Logger::ChangeExt(char* path, char* newext) {
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath(path, drive, dir, fname, ext);
	sprintf(path, "%s%s%s%s", drive, dir, fname, newext);
}