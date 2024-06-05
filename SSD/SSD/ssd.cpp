#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <stdexcept>

using namespace std;


class SSD {
public:
	static const int COMMAND_WRITE = 0x1;
	static const int COMMAND_READ = 0x2;

	void write(int address, int data) {
		string ssdData[100];

		if (address < 0 || address >= 100)
			throw out_of_range("address range is 0 <= address <= 99");

		/* First write case */
		ifstream checkFile(NAND);
		if (!checkFile.good()) {
			ofstream firstFile(NAND);
			for (int i = 0; i < 100; i++) {
				firstFile << "0" << endl;
			}
			firstFile.close();
		}
		checkFile.close();

		/* Get data */
		ifstream inFile(NAND);
		if (inFile.is_open()) {
			string line;
			int cnt = 0;
			string readData;
			while (getline(inFile, line)) {
				ssdData[cnt++] = line;
			}
		}
		inFile.close();

		/* Write data */
		stringstream dataToHex;
		dataToHex << std::hex << data;
		ssdData[address] = dataToHex.str();
		ofstream outFile(NAND);
		if (outFile.is_open()) {
			for (int i = 0; i < 100; i++) {
				outFile << ssdData[i] << endl;
			}
		}
		outFile.close();
	}

	int read(int address) {
		string ssdData[100];
		int data = 0;

		if (address < 0 || address >= 100)
			throw out_of_range("address range is 0 < address < 100");

		ifstream inFile(NAND);
		if (inFile.is_open()) {
			string line;
			int cnt = 0;
			string readData;
			while (getline(inFile, line)) {
				ssdData[cnt++] = line;
			}
		}
		inFile.close();

		ofstream outFile(OUTPUT);
		if (outFile.is_open()) {
			outFile << ssdData[address] << endl;

		}
		outFile.close();

		return 0;
	}

private:
	const string OUTPUT = "result.txt";
	const string NAND = "nand.txt";
};