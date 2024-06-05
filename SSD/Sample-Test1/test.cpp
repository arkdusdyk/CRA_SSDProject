#include <stdexcept>
#include <memory>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <windows.h>

#include "../SSD/ICommand.h"
#include "../SSD/Invoker.cpp"
#include "../SSD/Write.cpp"
#include "../SSD/Read.cpp"

class SSDFIxture : public testing::Test {
public:
    const string OUTPUT = "result.txt";
    const string NAND = "nand.txt";

    SSD ssd;
    CommandInvoker invoker{ &ssd };
    void SetUp() override {
        invoker.addCommand(std::move(std::make_unique<WriteCommand>()));
        invoker.addCommand(std::move(std::make_unique<ReadCommand>()));
    }

    void TearDown() override {
        LPCWSTR nandPath = L"nand.txt";
        LPCWSTR outputPath = L"result.txt";
        DeleteFile(nandPath);
        DeleteFile(outputPath);
    };
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
        }, ssd_exception);
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
        }, ssd_exception);
}

TEST_F(SSDFIxture, SsdBrokenFile) {
    ofstream testFile(NAND);
    for (int i = 0; i < 1; i++) {
        testFile << "0" << endl;
    }
    testFile.close();

    EXPECT_THROW({
         ssd.read(0);
        }, ssd_exception);
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
   
    int expectedData = 0;
    EXPECT_THAT(ret, testing::Eq(expectedData));
}

TEST_F(SSDFIxture, CommandInvokerWrite100) {
    int argc = 4;
    char* argv[] = { "ssd.exe", "W", "100", "0xdeadbeef" };

    EXPECT_THROW({
         invoker.executeCommands(argc, argv);
        }, ssd_exception);
}

TEST_F(SSDFIxture, CommandInvokerRead100) {
    int argc = 3;
    char* argv[] = { "ssd.exe", "R", "100"};

    EXPECT_THROW({
         invoker.executeCommands(argc, argv);
        }, ssd_exception);
}
