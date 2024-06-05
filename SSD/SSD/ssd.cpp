#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>

using namespace std;


class ssd_exception : public std::exception {
private:
	std::string what_message;
public:
	ssd_exception(char const* const message) {
		what_message = message;
	}
	const char* what() const override
	{
		return what_message.c_str();
	}
};

class SSD {
public:
	static const int COMMAND_WRITE = 0x1;
	static const int COMMAND_READ = 0x2;

	void write(int address, int data) {
		vector<string> ssdData;

		if (address < 0 || address >= MAX_ADDRESS)
			throw ssd_exception("address range is 0 <= address <= 99");

		checkDataInit();
		ssdData = getSsdData();

		stringstream dataToHex;
		dataToHex << std::hex << data;
		ssdData[address] = dataToHex.str();
		setSsdData(ssdData);
	}

	int read(int address) {
		vector<string> ssdData;
		int data = 0;

		if (address < 0 || address >= MAX_ADDRESS)
			throw ssd_exception("address range is 0 <= address <= 99");

		checkDataInit();
		ssdData = getSsdData();
		writeResult(ssdData[address]);

		return stoul(ssdData[address], nullptr, 16);
	}

private:
	const string OUTPUT = "result.txt";
	const string NAND = "nand.txt";
	const int MAX_ADDRESS = 100;

	void checkDataInit() {
		ifstream checkFile(NAND);

		if (!checkFile.good()) {
			ofstream firstFile(NAND);
			for (int i = 0; i < MAX_ADDRESS; i++) {
				firstFile << "0" << endl;
			}
			firstFile.close();
		}
		checkFile.close();
	}

	vector<string> getSsdData() {
		vector<string> data;
		ifstream inFile(NAND);

		if (inFile.is_open()) {
			string line;
			while (getline(inFile, line)) {
				data.push_back(line);
			}
		}
		inFile.close();

		if (data.size() != MAX_ADDRESS) {
			throw ssd_exception("File Broken");
		}

		return data;
	}

	void setSsdData(vector<string> data) {
		ofstream outFile(NAND);

		if (outFile.is_open()) {
			for (int i = 0; i < MAX_ADDRESS; i++) {
				outFile << data[i] << endl;
			}
		}
		outFile.close();
	}

	void writeResult(string value) {
		ofstream outFile(OUTPUT);

		if (outFile.is_open()) {
			outFile << value << endl;
		}
		outFile.close();
	}
};