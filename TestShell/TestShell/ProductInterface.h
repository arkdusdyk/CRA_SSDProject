#include <string>
#include <iostream>
#include <fstream>
using namespace std;

__interface IProduct
{
	virtual void Write(int addr, string value) = 0;
	virtual string Read(int addr) = 0;
};

class SSDProtocol : public IProduct
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

class createProductFactory {
public:
	createProductFactory(string productType) {
		createProduct(productType);
	}

	void createProduct(string productType) {
		if (productType == "SSD")
		{
			product = new SSDProtocol;
		}
	}

	IProduct* getProduct() {
		return product;
	}


private:
	IProduct* product = nullptr;
};