#include <iostream>

using namespace std;

class SSD {
public:
	int checkParameter(char*, int& command, int& address) {
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
	int fileOpen(string fileName) {
		return 0;
	}
};