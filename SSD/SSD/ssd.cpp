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
		return 0;
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