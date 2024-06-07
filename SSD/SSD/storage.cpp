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
		CommandSet cmd = { COMMAND_WRITE, address, data };

		setCommandList(cmd);
	}

	int read(int address) override {
		CommandSet cmd = { COMMAND_READ, address, 0, 0 };

		setCommandList(cmd);

		return 0;
	}

	void erase(int address, int size) override {
		CommandSet cmd = { COMMAND_ERASE, address, 0, size };

		setCommandList(cmd);
	}

	void flush() override {
		vector<CommandSet> cmdset;
		cmdset = getCommandList();

		for (CommandSet cmd : cmdset) {
			switch (cmd.cmdOpcode) {
			case COMMAND_READ:
				cmdRead(cmd);
				break;
			case COMMAND_WRITE:
				cmdWrite(cmd);
				break;
			case COMMAND_ERASE:
				cmdErase(cmd);
				break;
			default:
				cout << "Error" << endl;
			}
		}
	}

private:
	const string OUTPUT = "result.txt";
	const string NAND = "nand.txt";
	const string CMDFILE = "cmdlist.txt";
	static constexpr int SSD_CAPACITY = 100;
	static constexpr int MIN_LBA = 0;
	static constexpr int MAX_LBA = (SSD_CAPACITY -1);

	void checkingValidLba(int address, int size)
	{
		if (address < MIN_LBA || (address + size - 1) > MAX_LBA)
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

	void cmdWrite(CommandSet cmd) {
		vector<string> ssdData;
		int address = cmd.address;
		int data = cmd.data;

		checkingValidLba(address, 1);

		checkDataInit();
		ssdData = getSsdData();
		ssdData[address] = IntToHexUppercaseString(data);
		setSsdData(ssdData);
	}

	int cmdRead(CommandSet cmd) {
		vector<string> ssdData;
		int address = cmd.address;

		checkingValidLba(address, 1);

		checkDataInit();
		ssdData = getSsdData();
		writeResult(ssdData[address]);

		return stoul(ssdData[address], nullptr, 16);
	}

	void cmdErase(CommandSet cmd) {
		vector<string> ssdData;
		int address = cmd.address;
		int size = cmd.size;

		checkingValidLba(address, size);
		checkDataInit();
		ssdData = getSsdData();
		for (int i = address; i < address + size; i++) {
			ssdData[address] = IntToHexUppercaseString(0);
		}
		setSsdData(ssdData);
	}

	void setCommandList(CommandSet cmd) {
		vector<CommandSet> cmdlist;

		cmdlist = getCommandList();
		if (cmdlist.size() > 10) {
			flush();
		}
		cmdlist.push_back(cmd);

		ofstream cmdFile(CMDFILE);
		for (auto it = cmdlist.begin(); it != cmdlist.end(); it++) {
			string cmdStr;
			CommandSet tempCmd = *it;
			switch (tempCmd.cmdOpcode) {
			case COMMAND_READ:
				cmdStr = "R ";
				break;
			case COMMAND_WRITE:
				cmdStr = "W ";
				break;
			case COMMAND_ERASE:
				cmdStr = "E ";
				break;
			default:
				cout << "Unknown" << endl;
				break;
			}
			cmdStr += to_string(tempCmd.address) + " ";
			cmdStr += to_string(tempCmd.data) + " ";
			cmdStr += to_string(tempCmd.size);
			cmdFile << cmdStr << endl;
		}

		cmdFile.close();
	}

	vector<CommandSet> getCommandList() {
		ifstream cmdFile(CMDFILE);
		vector<CommandSet> cmdlist;

		if (!cmdFile.good()) {
			cmdFile.close();
			return cmdlist;
		}

		string line;
		while (getline(cmdFile, line)) {
			istringstream iss(line);
			CommandSet tempCmd;
			char tempStr;

			iss >> tempStr >> tempCmd.address >> tempCmd.data >> tempCmd.size;

			switch (tempStr) {
			case 'R':
				tempCmd.cmdOpcode = COMMAND_READ;
				break;
			case 'W':
				tempCmd.cmdOpcode = COMMAND_WRITE;
				break;
			case 'E':
				tempCmd.cmdOpcode = COMMAND_ERASE;
				break;
			default:
				cout << "Unknown Command" << endl;
				break;
			}
			cmdlist.push_back(tempCmd);
		}

		return cmdlist;
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