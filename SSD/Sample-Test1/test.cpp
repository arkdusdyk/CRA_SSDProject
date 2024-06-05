#include <stdexcept>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../SSD/ICommand.h"
#include "../SSD/Invoker.cpp"
#include "../SSD/Write.cpp"
#include "../SSD/Read.cpp"

class SSDFIxture : public testing::Test {
public:
    SSD ssd;
    CommandInvoker invoker{ &ssd };
    void SetUp()
    {
        invoker.addCommand(new WriteCommand);
        invoker.addCommand(new ReadCommand);
    }
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

TEST_F(SSDFIxture, CommandInvokerWrite0) {
    int argc = 4;
    char* argv[] = { "ssd.exe", "W", "0", "0xdeadbeef"};
    int ret = invoker.executeCommands(argc, argv);

    EXPECT_THAT(ret, testing::Eq(ICommand::COMMAND_VALIDATION_SUCCESS));
}

TEST_F(SSDFIxture, CommandInvokerRead0) {
    int argc = 3;
    char* argv[] = { "ssd.exe", "R", "0"};
    int ret = invoker.executeCommands(argc, argv);
   
    int expectedData= 0;
    EXPECT_THAT(ret, testing::Eq(expectedData));
}

TEST_F(SSDFIxture, CommandInvokerWrite100) {
    int argc = 4;
    char* argv[] = { "ssd.exe", "W", "100", "0xdeadbeef" };

    EXPECT_THROW({
         invoker.executeCommands(argc, argv);
        }, std::out_of_range);
}

TEST_F(SSDFIxture, CommandInvokerRead100) {
    int argc = 3;
    char* argv[] = { "ssd.exe", "R", "100"};

    EXPECT_THROW({
         invoker.executeCommands(argc, argv);
        }, std::out_of_range);
}
