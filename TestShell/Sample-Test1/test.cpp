#include <stdexcept>

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
	TestShell testShell;
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