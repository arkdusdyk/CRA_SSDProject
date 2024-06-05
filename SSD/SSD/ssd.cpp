#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "ssdexcept.h"

using namespace std;

struct CommandSet
{
	int cmdOpcode;
	int address;
	int data;
};

class SSD {
public:
	static const int COMMAND_WRITE = 0x1;
	static const int COMMAND_READ = 0x2;

	void write(int address, int data) {
		vector<string> ssdData;

		checkingValidLba(address);

		checkDataInit();
		ssdData = getSsdData();
		ssdData[address] = IntToHexString(data);
		setSsdData(ssdData);
	}

	int read(int address) {
		vector<string> ssdData;

		checkingValidLba(address);

		checkDataInit();
		ssdData = getSsdData();
		writeResult(ssdData[address]);

		return stoul(ssdData[address], nullptr, 16);
	}

private:
	const string OUTPUT = "result.txt";
	const string NAND = "nand.txt";
	static constexpr int SSD_CAPACITY = 100;
	static constexpr int MIN_LBA = 0;
	static constexpr int MAX_LBA = (SSD_CAPACITY -1);

	void checkingValidLba(int address)
	{
		if (address < MIN_LBA || address > MAX_LBA)
		{
			string errorMessage = "address range is ";
			errorMessage += std::to_string(MIN_LBA) + " <= address <= " + std::to_string(MAX_LBA);
			throw ssd_exception(errorMessage);
		}
	}

	void checkDataInit() {
		ifstream checkFile(NAND);

		if (checkFile.good()) {
			checkFile.close();
			return;
		}

		ofstream firstFile(NAND);
		for (int i = MIN_LBA; i <= MAX_LBA; i++) {
			firstFile << "0" << endl;
		}
		firstFile.close();
		checkFile.close();
	}

	vector<string> getSsdData() {
		vector<string> data;
		ifstream inFile(NAND);

		if (!inFile.is_open()) {
			throw ssd_exception("Cannot Open File");
		}

		string line;
		while (getline(inFile, line)) {
			data.push_back(line);
		}
		inFile.close();

		if (data.size() != SSD_CAPACITY) {
			throw ssd_exception("File Broken");
		}

		return data;
	}

	void setSsdData(vector<string> data) {
		ofstream outFile(NAND);

		if (!outFile.is_open()) {
			throw ssd_exception("Cannot Open File");
		}

		for (int i = MIN_LBA; i <= MAX_LBA; i++) {
			outFile << data[i] << endl;
		}

		outFile.close();
	}

	void writeResult(string value) {
		ofstream outFile(OUTPUT);

		if (!outFile.is_open()) {
			throw ssd_exception("Cannot Open File");
		}
		
		outFile << value << endl;

		outFile.close();
	}

	std::string IntToHexString(int data)
	{
		std::stringstream dataToHex;
		dataToHex << std::hex << data;
		return dataToHex.str();
	}

};