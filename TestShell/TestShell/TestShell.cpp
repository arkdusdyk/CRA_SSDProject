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
private:
	IProduct* product;
};