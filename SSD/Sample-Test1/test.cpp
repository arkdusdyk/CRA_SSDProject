#include <stdexcept>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../SSD/ssd.cpp"

class SSDFIxture : public testing::Test {
public:
    SSD ssd;
};

TEST_F(SSDFIxture, SsdWrite0) {
    int address = 0;
    int data = 0xdeadbeef;
    ssd.write(address, data);
}

TEST_F(SSDFIxture, SsdWrite99) {
    int address = 0;
    int data = 0xdeadbeef;
    ssd.write(address, data);
}

TEST_F(SSDFIxture, SsdWrite100) {
    int address = 100;
    int data = 0xdeadbeef;
    EXPECT_THROW({
         ssd.write(address, data);
        }, std::out_of_range);
}

TEST_F(SSDFIxture, SsdRead0) {
    int address = 0;
    int expected = 0x00000000;
    int result = ssd.read(address);
    EXPECT_THAT(expected, testing::Eq(expected));
}

TEST_F(SSDFIxture, SsdRead99) {
    int address = 99;
    int expected = 0x00000000;
    int result = ssd.read(address);
    EXPECT_THAT(expected, testing::Eq(expected));
}

TEST_F(SSDFIxture, SsdRead100) {
    int address = 100;
    int expected = 0x00000000;
    EXPECT_THROW({
         ssd.read(address);
        }, std::out_of_range);
}

TEST_F(SSDFIxture, SsdCommandCheckParamWrite0) {
    int argc = 4;
    char* argv[] = { "ssd.exe", "W", "0", "0xdeadbeef"};
    CommandSet cmd;
    int result = ssd.checkParameter(argc, argv, cmd);

    int expectedAddress = 0;
    int expectedData = 0xdeadbeef;
    EXPECT_THAT(cmd.cmdOpcode, testing::Eq(SSD::COMMAND_WRITE));
    EXPECT_THAT(cmd.address, testing::Eq(expectedAddress));
    EXPECT_THAT(cmd.data, testing::Eq(0xdeadbeef));
    EXPECT_THAT(result, testing::Eq(SSD::COMMAND_VALIDATION_SUCCESS));
}

TEST_F(SSDFIxture, SsdCommandCheckParamRead0) {
    int argc = 3;
    char* argv[] = { "ssd.exe", "R", "0"};
    CommandSet cmd;
    int result = ssd.checkParameter(argc, argv, cmd);

    int expectedAddress = 0;
    EXPECT_THAT(cmd.cmdOpcode, testing::Eq(SSD::COMMAND_READ));
    EXPECT_THAT(cmd.address, testing::Eq(expectedAddress));
    EXPECT_THAT(result, testing::Eq(SSD::COMMAND_VALIDATION_SUCCESS));
}


TEST_F(SSDFIxture, SsdCommandCheckParamWrite100) {
    int argc = 4;
    char* argv[] = { "ssd.exe", "W", "100", "0xdeadbeef" };
    CommandSet cmd;
    int result = ssd.checkParameter(argc, argv, cmd);

    int expectedAddress = 100;
    int expectedData = 0xdeadbeef;
    EXPECT_THAT(cmd.cmdOpcode, testing::Eq(SSD::COMMAND_WRITE));
    EXPECT_THAT(cmd.address, testing::Eq(expectedAddress));
    EXPECT_THAT(cmd.data, testing::Eq(0xdeadbeef));
    EXPECT_THAT(result, testing::Eq(SSD::COMMAND_VALIDATION_SUCCESS));
}

TEST_F(SSDFIxture, SsdCommandCheckParamRead100) {
    int argc = 3;
    char* argv[] = { "ssd.exe", "R", "100" };
    CommandSet cmd;
    int result = ssd.checkParameter(argc, argv, cmd);

    int expectedAddress = 100;
    EXPECT_THAT(cmd.cmdOpcode, testing::Eq(SSD::COMMAND_READ));
    EXPECT_THAT(cmd.address, testing::Eq(expectedAddress));
    EXPECT_THAT(result, testing::Eq(SSD::COMMAND_VALIDATION_SUCCESS));
}
