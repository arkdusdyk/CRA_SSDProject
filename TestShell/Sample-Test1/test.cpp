#include <stdexcept>
#include <iostream>
#include <ostream>

#include "../TestShell/TestShell.cpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace testing;
using namespace std;

class ProductMock : public IProtocol {
public:
	MOCK_METHOD(void, Write, (int addr, string value), (override));
	MOCK_METHOD(string, Read, (int addr), (override));
};

class TestShellFixture : public Test {
public:
	ProductMock pMock;
  SSDProtocol protocol;
	TestShell testShell;
	const int MAX_CELL_COUNT = 100;

	bool CheckFullRead_OneExpect(string expected) {
		vector<string> readvalue = testShell.FullRead();
		for (int address = 0; address < MAX_CELL_COUNT; address++)
		{
			if (readvalue[address] != expected)
				return false;
		}
		return true;
	}

	bool CheckFullRead_VectorExpect(vector<string> expecteds) {
		vector<string> readvalue = testShell.FullRead();
		for (int address = 0; address < MAX_CELL_COUNT; address++)
		{
			if (readvalue[address] != expecteds[address])
				return false;
		}
		return true;
	}
};


TEST_F(TestShellFixture, ReadFailTest) {
	EXPECT_THROW(pMock.Read(110), exception);
}

TEST_F(TestShellFixture, writeWrongAddrWrite) {	
	EXPECT_THROW(testShell.Write(-1, "0x123456AB"); , exception);
}

TEST_F(TestShellFixture, writeA) {
	int address = 1;
	string expected = "0x123456AB";

	EXPECT_CALL(pMock, Read(address)).WillRepeatedly(Return(expected));
	
	testShell.Write(address, "0x123456AB");
	EXPECT_THAT(testShell.Read(address), Eq(expected));
}

TEST_F(TestShellFixture, writeAwriteB) {
	int address = 1;
	string firstInput = "0x123456AB";
	string expected = "0xABCDEFGH";

	EXPECT_CALL(pMock, Read(address)).WillRepeatedly(Return(expected));

	EXPECT_CALL(pMock, Write(address, _)).Times(2);

	testShell.Write(address, firstInput);
	testShell.Write(address, expected);
	EXPECT_THAT(testShell.Read(address), Eq(expected));
}


TEST_F(TestShellFixture, writeInvalidValue) {
	int address = 1;
	string invalidInput = "0x1234GGGG";

	EXPECT_THROW(pMock.Write(address, invalidInput), exception);
}

TEST_F(TestShellFixture, fullWrite) {
	int address = 1;
	string expected = "0xABCDEFGH";
	EXPECT_CALL(pMock, Read(address)).Times(100).WillRepeatedly(Return(expected));

	testShell.FulllWrite(expected);
	EXPECT_TRUE(CheckFullRead_OneExpect(expected));
}

TEST_F(TestShellFixture, fullRead) {
	int address = 1;
	vector<string> expecteds(100, "0x00000000");
	expecteds[0] = "0xABCDEFGH";
	expecteds[1] = "0x123456AB";
	expecteds[2] = "0x12345ABC";

	testShell.FulllWrite("0x12345678");

	EXPECT_CALL(pMock, Read(address)).Times(100)
		.WillOnce(Return("0xABCDEFGH"))
		.WillOnce(Return("0x123456AB"))
		.WillOnce(Return("0x12345ABC"))
		.WillRepeatedly(Return("0x00000000"));
	
	EXPECT_TRUE(CheckFullRead_VectorExpect(expecteds));

TEST_F(TestShellFixture, SSDProtocolRead) {
	ofstream file;
	string expected = "0x11223344";
	string actual;
	file.open("result.txt");
	file << "0x11223344" << endl;
	file.close();
	actual = protocol.Read(1);
	EXPECT_EQ(actual, expected);
}