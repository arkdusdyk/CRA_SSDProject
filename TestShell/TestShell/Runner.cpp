#include <stdexcept>
#include <iostream>
#include <string>
#include "TestShell.cpp"
#include "CommandParser.cpp"

class Runner {
public:
	void runScenario(string file_name) {
		filename = file_name;
	}
private:
	string filename;
};