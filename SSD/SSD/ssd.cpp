#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <vector>

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
		vector<string> ssdData;

		if (address < 0 || address >= 100)
			throw out_of_range("address range is 0 <= address <= 99");

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

		if (address < 0 || address >= 100)
			throw out_of_range("address range is 0 <= address <= 99");

		checkDataInit();
		ssdData = getSsdData();
		writeResult(ssdData[address]);

		return stoul(ssdData[address], nullptr, 16);
	}

private:
	const string OUTPUT = "result.txt";
	const string NAND = "nand.txt";

	void checkDataInit() {
		ifstream checkFile(NAND);

		if (!checkFile.good()) {
			ofstream firstFile(NAND);
			for (int i = 0; i < 100; i++) {
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
			int cnt = 0;
			string readData;
			while (getline(inFile, line)) {
				data.push_back(line);
			}
		}
		inFile.close();

		return data;
	}

	void setSsdData(vector<string> data) {
		ofstream outFile(NAND);

		if (outFile.is_open()) {
			for (int i = 0; i < 100; i++) {
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