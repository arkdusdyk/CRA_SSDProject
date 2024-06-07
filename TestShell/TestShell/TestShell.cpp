#include <iostream>
#include <vector>
#include <regex>

#include "ProductInterface.h"
#include "TestShell_Exception.h"

using namespace std;

class TestShell {
public:
	void setProduct(IProduct* iproduct) {
		product = iproduct;
	}
	void createProduct(string device) {
		createProductFactory cPF(device);
		setProduct(cPF.getProduct());
	}
	void Write(int arr, string value) {
		checkInvalid_LBA(arr);
		checkInvalid_Value(value);
		product->Write(arr, value);
	}

	string Read(int arr) {
		checkInvalid_LBA(arr);
		string result = product->Read(arr);
		cout << result << endl;
		return result;
	}

	void Erase(int arr, int size) {
		checkInvalid_LBA(arr);
		
		int tmp = size;
		for (int i = 0; i < (size/10 + 1); i++) {
			if (tmp > 10) {
				product->Erase(arr + i * 10, 10);
				tmp -= 10;
			}
			else {
				product->Erase(arr + i * 10, tmp);
			}
		}
	}

	void EraseRange(int arr, int endArr) {
		checkInvalid_LBA(arr);
		checkInvalid_LBA(endArr);
		Erase(arr, endArr - arr);
	}

	void FulllWrite( string value) {
		for (int lba = 0; lba < 100; lba++) {
			product->Write(lba, value);
		}
	}

	vector<string> FullRead() {
		vector<string> results;
		for (int lba = 0; lba < MAX_LBA; lba++) {
			results.push_back(product->Read(lba));
			cout << results[lba] << endl;
		}
		return results;
	}

	bool testApp1() {
		bool ret = false;
		FulllWrite(TEST_PATTERN_TESTAPP1);
		vector<string> readResult = FullRead();
		if (readResult.size() != MAX_LBA) return false;

		for (int lba = 0; lba < MAX_LBA; lba++) {
			if (readResult[lba] != TEST_PATTERN_TESTAPP1) return false;
		}
		return true;
	}

	bool testApp2() {
		for (int cnt = 0; cnt < 30; cnt++) {
			for (int lbaAddress = 0; lbaAddress <= 5; lbaAddress++) {
				product->Write(lbaAddress, "0xAAAABBBB");
			}
		}

		for (int lbaAddress = 0; lbaAddress <= 5; lbaAddress++) {
			product->Write(lbaAddress, "0x12345678");
		}

		for (int lbaAddress = 0; lbaAddress <= 5; lbaAddress++) {
			string readValue = product->Read(lbaAddress);

			if (readValue != TEST_PATTERN_TESPAPP2) return false;

			cout << readValue << "\n";
		}

		return true;
	}

	void Help() {
		for (string help : helpText) {
			cout << help << endl;
		}
	}

private:
	const int MIN_LBA = 0;
	const int MAX_LBA = 100;
	const string TEST_PATTERN_TESTAPP1 = ("0xAABBCCDD");
	const string TEST_PATTERN_TESPAPP2 = "0x12345678";

	const vector<string> helpText = {
		"주요 명령어",
		"1. write (예) write LBA data : LBA 에 data를 기록",
		"2. read (예) read LBA : LBA 를 읽음)",
		"3. exit (예) exit : shell 을 종료)",
		"4. help (예) help : 주요 명령어의 사용방법 출력)",
		"5. fullwrite (예) fullwrite data : 모든 LBA(0~99)까지 data 를 write 수행)",
		"6. fullread (예) fullread : 모든 LBA(0~99) 까지 read 수행)"
	};
	IProduct* product;

	void checkInvalid_LBA(int arr) {
		if (arr < MIN_LBA || arr > MAX_LBA)
			throw wrongAdrress_exception();
	}

	void checkInvalid_Value(string value) {
		std::regex reg("0x[0-9A-F]{8}$");
		if (!regex_match(value, reg))
			throw wrongValue_exception();
	}
};
