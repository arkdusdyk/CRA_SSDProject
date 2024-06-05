#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "ssd.cpp"

using namespace std;

struct CommandSet
{
	int cmdOpcode;
	int address;
	int data;
};

class ICommand {
public:
	static const int COMMAND_VALIDATION_SUCCESS = 0;
	static const int COMMAND_VALIDATION_FAIL = -1;

	virtual string getCommandCode() const = 0;
	virtual int execute(int argc, char* argv[], SSD *ssd) = 0;

protected:
	bool IsValidData(const string value) {
		if (value.size() != 10)
			return false;

		if (value.substr(0, 2) != "0x")
			return false;

		if (!isHexadecimal(value.substr(2)))
		{
			return false;
		}

		return true;
	}

	bool isDecimal(const std::string& str) {
		if (str.empty()) return false;

		for (char c : str) {
			if (!(std::isdigit(c))) {
				return false;
			}
		}

		return true;
	}

	bool isHexadecimal(const std::string& str) {
		if (str.empty()) return false;

		for (char c : str) {
			if (!(std::isdigit(c) || (std::isalpha(c) && std::toupper(c) >= 'A' && std::toupper(c) <= 'F'))) {
				return false;
			}
		}

		return true;
	}

	int checkParameter(int argc, char* argv[], CommandSet& commandset) {

		if (argc >= 3 && !isDecimal(string(argv[2])))
		{
			return COMMAND_VALIDATION_FAIL;
		}

		if (argc >= 4 && !IsValidData(string(argv[3])))
		{
			return COMMAND_VALIDATION_FAIL;
		}

		if (argc == 4 && string(argv[1]) == "W")
		{
			commandset.cmdOpcode = SSD::COMMAND_WRITE;
			commandset.address = std::stoi(string(argv[2]));
			commandset.data = static_cast<int>(std::stoll(string(argv[3]), nullptr, 16));
		}
		else if (argc == 3 && string(argv[1]) == "R")
		{
			commandset.cmdOpcode = SSD::COMMAND_READ;
			commandset.address = std::stoi(string(argv[2]));
			commandset.data = -1;
		}
		else
			return COMMAND_VALIDATION_FAIL;

		return COMMAND_VALIDATION_SUCCESS;
	}
};