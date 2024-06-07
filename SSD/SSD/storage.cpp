#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <vector>
#include <windows.h>

#include "ssdexcept.h"

using namespace std;

struct CommandSet
{
	int cmdOpcode;
	int address;
	int data;
	int size;
};

const enum DeviceType {
	TYPE_SSD,
};

interface Storage {
	virtual void write(int address, int data) = 0;
	virtual void read(int address) = 0;
	virtual void erase(int address, int size) = 0;
	virtual void flush() = 0;
};

class SSD : public Storage {
public:
	static const int COMMAND_WRITE = 0x1;
	static const int COMMAND_READ = 0x2;
	static const int COMMAND_ERASE = 0x3;
	static const int COMMAND_FLUSH = 0x4;

	void write(int address, int data) override {
		CommandSet cmd = { COMMAND_WRITE, address, data };

		setCommandList(cmd);
	}

	void read(int address) override {
		CommandSet cmd = { COMMAND_READ, address, 0, 0 };

		setCommandList(cmd);
	}

	void erase(int address, int size) override {
		CommandSet cmd = { COMMAND_WRITE, address, 0, size };

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
				throw ssd_exception("Unknown Command");
				break;
			}
		}
		DeleteFile(wstring(CMDFILE.begin(), CMDFILE.end()).c_str());
	}

private:
	const string OUTPUT = "result.txt";
	const string NAND = "nand.txt";
	const string CMDFILE = "cmdlist.txt";
	static constexpr int SSD_CAPACITY = 100;
	static constexpr int MIN_LBA = 0;
	static constexpr int MAX_LBA = (SSD_CAPACITY -1);
	static constexpr int CLEAN_PAGE_DATA = 0;

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

	string IntToHexUppercaseString(int data)
	{
		std::stringstream dataToHex;
		dataToHex << "0x" << std::hex << std::setw(8) << std::setfill('0') << std::uppercase << data;
		return dataToHex.str();
	}

	void writeCommand(vector<CommandSet> cmdlist) {
		ofstream cmdFile(CMDFILE);
		if (!cmdFile.is_open()) {
			throw ssd_exception("Cannot Open File");
		}

		for (CommandSet tempCmd : cmdlist) {
			string cmdStr;

			cmdStr = to_string(tempCmd.cmdOpcode) + " ";
			cmdStr += to_string(tempCmd.address) + " ";
			cmdStr += to_string(tempCmd.data) + " ";
			cmdStr += to_string(tempCmd.size);
			cmdFile << cmdStr << endl;
		}
		cmdFile.close();
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

	void cmdRead(CommandSet cmd) {
		vector<string> ssdData;
		int address = cmd.address;

		checkingValidLba(address, 1);

		checkDataInit();
		ssdData = getSsdData();
		writeResult(ssdData[address]);
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
		cmdlist.push_back(cmd);
		writeCommand(cmdlist);
		if (cmdlist.size() >= 10) {
			flush();
		}
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

			iss >> tempCmd.cmdOpcode >> tempCmd.address >> tempCmd.data >> tempCmd.size;
			cmdlist.push_back(tempCmd);
		}
		cmdFile.close();

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