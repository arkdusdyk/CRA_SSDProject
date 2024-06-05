#include <string>
#include <iostream>
#include <fstream>
#include <Windows.h>
using namespace std;


__interface IProduct
{
	virtual void Write(int addr, string value) = 0;
	virtual string Read(int addr) = 0;
};

class SSDProduct : public IProduct
{
public:
	SSDProduct() {
		GetCurrentDirectoryA(256, curPath);
	}
	virtual void Write(int addr, string value) override {		
		string cmd = curPath;
		cmd.append(mExecuteName + " W " + std::to_string(addr) + " " + value);
		if (system(cmd.c_str()) < 0)
			cout << "실행파일을 실행하지 못했습니다." << endl;
		
	}

	virtual string Read(int addr) override {
		string cmd = curPath;
		cmd.append(mExecuteName + " R " + std::to_string(addr));
		if (system(cmd.c_str()) < 0)
			cout << "실행파일을 실행하지 못했습니다." << endl;
		ifstream readFile;
		string result;
		readFile.open(mReadFileName);
		if (readFile.is_open()) {
			string buf;
			while (getline(readFile, buf)) {
				result.append(buf);
			}
		}
		else
		{
			cout << "결과 파일을 읽지 못했습니다." << endl;
		}
		readFile.close();
		return result;
	}
private:
	char curPath[256];

	string mReadFileName = "result.txt";
	string mExecuteName = "\\..\\..\\SSD\\x64\\Debug\\ssd.exe";
};

class createProductFactory {
public:
	createProductFactory(string productType) {
		createProduct(productType);
	}

	void createProduct(string productType) {
		if (productType == "SSD")
		{
			product = new SSDProduct;
		}
	}

	IProduct* getProduct() {
		return product;
	}


private:
	IProduct* product = nullptr;
};
