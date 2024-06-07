#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <vector>

#include "ssdexcept.h"

using namespace std;

struct CommandSet
{
	int cmdOpcode;
	int address;
	int data;
	int size;
};

#define interface struct

const enum DeviceType {
	TYPE_SSD,
};

interface Storage {
	virtual void write(int address, int data) = 0;
	virtual int read(int address) = 0;
	virtual void erase(int address, int size) = 0;
	virtual void flush() = 0;
};

class SSD : public Storage {
public:
	static const int COMMAND_WRITE = 0x1;
	static const int COMMAND_READ = 0x2;
	static const int COMMAND_ERASE = 0x3;
	static const int COMMAND_FLUSH = 0x4;
	static const int CLEAN_PAGE_DATA = 0;

	void write(int address, int data) override {
		vector<string> ssdData;

		checkingValidLba(address);

		checkDataInit();
		ssdData = getSsdData();
		ssdData[address] = IntToHexUppercaseString(data);
		setSsdData(ssdData);
	}

	int read(int address) override {
		vector<string> ssdData;

		checkingValidLba(address);

		checkDataInit();
		ssdData = getSsdData();
		writeResult(ssdData[address]);

		return stoul(ssdData[address], nullptr, 16);
	}

	void erase(int address, int size) override {
		checkingValidSize(address, size);
		for (int lba = address; lba < address + size; lba++)
		{
			write(lba, CLEAN_PAGE_DATA);
		}
	}

	void flush() override {
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

	void checkingValidSize(int address, int size)
	{		
		for (int lba = address; lba < address + size; lba++)
		{
			checkingValidLba(lba);
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
			firstFile << IntToHexUppercaseString(CLEAN_PAGE_DATA) << endl;
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

	std::string IntToHexUppercaseString(int data)
	{
		std::stringstream dataToHex;
		dataToHex << "0x" << std::hex << std::setw(8) << std::setfill('0') << std::uppercase << data;
		return dataToHex.str();
	}

};

interface Device {
	virtual Storage* setDevice(enum DeviceType) = 0;
};

class StorageDevice : public Device {
public:
	Storage* setDevice(enum DeviceType type) {
		if (type == TYPE_SSD) {
			return new SSD();
		}
		return nullptr;
	}

private:
	Storage* storage;
};