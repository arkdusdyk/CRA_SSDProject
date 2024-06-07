#include "pch.h"
#include "Logger.h"
#include "iostream"

void Logger::write_Log(string log) {
	cout << log << endl;
}

void write_Log(Logger* looger, string log) {
	looger->write_Log(log);
}