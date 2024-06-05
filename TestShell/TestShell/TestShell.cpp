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
		return product->Read(arr);
	}

	void FulllWrite( string value) {
		for (int lba = 0; lba < 100; lba++) {
			product->Write(lba, value);
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
	IProduct* product;
};