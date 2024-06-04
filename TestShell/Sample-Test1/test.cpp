#include <stdexcept>

#include "../TestShell/TestShell.cpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace testing;
using namespace std;


class MockTestShell : public TestShell {
public:
	MOCK_METHOD(void, Write, (int addr, string value), ());
	MOCK_METHOD(string, Read, (int addr), ());
	MOCK_METHOD(void, FullWrite, (string value), ());
	MOCK_METHOD(string, FullRead, (), ());
};

TEST(TestCaseName, TestName) {
	EXPECT_THAT(1, Eq(1));
}

TEST(ShellReadTest, ReadTest) {
	MockTestShell mock;

	EXPECT_CALL(mock, Read(0)).WillOnce(Return("Ox00000000"));

	cout << mock.Read(0) << "\n";
}

TEST(ShellReadTest, ReadFailTest) {
	MockTestShell mock;

	EXPECT_THROW(mock.Read(110), exception);

}

TEST(ShellReadTest, FullReadTest) {
	MockTestShell mock;

	EXPECT_CALL(mock, FullRead()).Times(1);
	EXPECT_CALL(mock, Read(_)).Times(100);

	cout << mock.FullRead() << "\n";
}