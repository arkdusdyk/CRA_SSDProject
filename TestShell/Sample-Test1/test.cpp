#include <stdexcept>

#include "../TestShell/TestShell.cpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace testing;


class Mock : public TestShell {
public:
	MOCK_METHOD(void, Write, (int addr, string value), (override));
	MOCK_METHOD(string, Read, (int addr), (override));
	MOCK_METHOD(void, FullWrite, (string value), (override));
	MOCK_METHOD(string, FullRead, , (override));
};


TEST(TestCaseName, TestName) {
	EXPECT_THAT(1, Eq(1));
}