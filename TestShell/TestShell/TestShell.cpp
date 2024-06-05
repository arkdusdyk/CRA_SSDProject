#include <iostream>
#include "ProductInterface.h"
#include <vector>

using namespace std;

class TestShell {
public:
	void setProduct(IProduct* iproduct) {
		product = iproduct;
	}

	void Write(int arr, string value) {
		product->Write(arr, value);
	}

	string Read(int arr) {
		string result = product->Read(arr);
		cout << result << endl;
		return product->Read(arr);
	}

	void FulllWrite( string value) {
		for (int lba = 0; lba < 100; lba++) {
			product->Write(lba, value);
		}
	}

	vector<string> FullRead() {
		vector<string> results;
		for (int lba = 0; lba < MAX_LBA; lba++) {
			results.push_back(iprotocol->Read(lba));
			cout << results[lba] << endl;
		}
		return results;
	}

	bool testApp1() {
		bool ret = false;
		FulllWrite(TEST_PATTERN);
		vector<string> readResult = FullRead();
		if (readResult.size() != MAX_LBA) return false;

		for (int lba = 0; lba < MAX_LBA; lba++) {
			if (readResult[lba] != TEST_PATTERN) return false;
		}
		return true;
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
	const int MAX_LBA = 100;
	const string TEST_PATTERN = ("0xAABBCCDD");
	IProduct* product;
};