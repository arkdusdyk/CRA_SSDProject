#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class SSD {
public:
	int checkParameter(char* args, int& command, int& address, int& data) {
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