#include <iostream>
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

	}

	virtual string Read(int addr) override {
		return "";
	}
};

class TestShell {
public:
	void Write(int arr, string value) {

	}

	string Read(int arr) {
		return "";
	}

	void FulllWrite( string value) {

	}

	vector<string> FullRead() {
		return { "" };
	}
};