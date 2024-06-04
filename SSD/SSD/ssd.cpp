#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct CommandSet
{
	int cmdOpcode;
	int address;
	int data;
};

class SSD {
public:
	static const int COMMAND_VALIDATION_SUCCESS = 0;
	static const int COMMAND_VALIDATION_FAIL = -1;

	static const int COMMAND_WRITE = 0x1;
	static const int COMMAND_READ = 0x2;

	int checkParameter(int argc, char* argv[], CommandSet& commandset) {
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

	void write(int address, int data) {

	}

	int read(int address) {
		int data = 0;
		return data;
	}

private:
	const string OUTPUT = "result.txt";
	const string NAND = "nand.txt";
};