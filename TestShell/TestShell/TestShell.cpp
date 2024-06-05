#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

__interface IProtocol
{
	virtual void Write(int addr, string value) = 0;
	virtual string Read(int addr) = 0;
};

class SSDProtocol : public IProtocol
{
public:
	virtual void Write(int addr, string value) override {
		string cmd;
		cmd.append(mExecuteName + " W " + std::to_string(addr) + " " + value);
		system(cmd.c_str());
	}

	virtual string Read(int addr) override {
		string cmd;
		cmd.append(mExecuteName + " R " + std::to_string(addr));
		system(cmd.c_str());
		ifstream readFile;
		string result;
		readFile.open(mReadFileName);
		if (readFile.is_open()) {
			string buf;
			while (getline(readFile, buf)) {
				result.append(buf);
			}
		}
		readFile.close();
		return result;
	}
private:
	string mReadFileName = "result.txt";
	string mExecuteName = "ssd.exe";
};

class TestShell {
public:
	void setProtocol(IProtocol *iprotocol) {
		iprotocol = iprotocol;
	}

	void Write(int arr, string value) {
		iprotocol->Write(arr, value); 
	}

	string Read(int arr) {
		return "";
	}

	void FulllWrite( string value) {
		for (int lba = 0; lba < 100; lba++) {
			iprotocol->Write(lba, value);
		}
	}

	vector<string> FullRead() {
		return { "" };
	}

	void testApp2() {
		for (int cnt = 0; cnt < 30; cnt++) {
			for (int lbaAddress = 0; lbaAddress <= 5; lbaAddress++) {
				iprotocol->Write(lbaAddress, "0xAAAABBBB");
			}
		}

		for (int lbaAddress = 0; lbaAddress <= 5; lbaAddress++) {
			iprotocol->Write(lbaAddress, "0x12345678");
		}

		for (int lbaAddress = 0; lbaAddress <= 5; lbaAddress++) {
			cout << iprotocol->Read(lbaAddress) << "\n";
		}
	}
private:
	IProtocol* iprotocol;
};