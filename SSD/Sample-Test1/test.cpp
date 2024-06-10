#include <stdexcept>
#include <memory>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <windows.h>

#include "../SSD/ICommand.h"
#include "../SSD/Invoker.cpp"
#include "../SSD/Write.cpp"
#include "../SSD/Read.cpp"
#include "../SSD/Erase.cpp"
#include "../SSD/Flush.cpp"

class SSDFIxture : public testing::Test {
public:
    const string OUTPUT = "result.txt";
    const string NAND = "nand.txt";

    Device* device = new StorageDevice();
    Storage* ssd = device->setDevice(TYPE_SSD);
    CommandInvoker invoker{ ssd };

    void SetUp() override {
        invoker.addCommand(std::move(std::make_unique<WriteCommand>()));
        invoker.addCommand(std::move(std::make_unique<ReadCommand>()));
        invoker.addCommand(std::move(std::make_unique<EraseCommand>()));
        invoker.addCommand(std::move(std::make_unique<FlushCommand>()));
    }

    void TearDown() override {
        LPCWSTR nandPath = L"nand.txt";
        LPCWSTR outputPath = L"result.txt";
        LPCWSTR cmdlistPath = L"cmdlist.txt";
        DeleteFile(nandPath);
        DeleteFile(outputPath);
        DeleteFile(cmdlistPath);
    };

    int readResult()
    {
        ifstream inFile(OUTPUT);

        if (!inFile.is_open()) {
            throw ssd_exception("Cannot Open File");
        }

        string line;
        getline(inFile, line);
        inFile.close();

        return std::stoul(line, nullptr, 16);
    }

    int makeReadCmd(char* address) {
        char* argv[3];

        argv[0] = "ssd.exe";
        argv[1] = "R";
        argv[2] = address;
        return invoker.executeCommands(3, argv);
    }

    int makeWriteCmd(char* address, char* value) {
        char* argv[4];

        argv[0] = "ssd.exe";
        argv[1] = "W";
        argv[2] = address;
        argv[3] = value;
        return invoker.executeCommands(4, argv);
    }

    int makeEraseCmd(char* address, char* size) {
        char* argv[4];

        argv[0] = "ssd.exe";
        argv[1] = "E";
        argv[2] = address;
        argv[3] = size;
        return invoker.executeCommands(4, argv);
    }

    int  makeFlushCmd() {
        char* argv[2];

        argv[0] = "ssd.exe";
        argv[1] = "F";
        return invoker.executeCommands(2, argv);
    }
};

TEST_F(SSDFIxture, SsdWrite0) {
    int address = 0;
    int data = 0xdeadbeef;
    ssd->write(address, data);
}

TEST_F(SSDFIxture, SsdWrite99) {
    int address = 0;
    int data = 0xdeadbeef;
    ssd->write(address, data);
}

TEST_F(SSDFIxture, SsdWrite100) {
    int address = 100;
    int data = 0xdeadbeef;
    EXPECT_THROW({
         ssd->write(address, data);
        }, ssd_exception);
}

TEST_F(SSDFIxture, SsdRead0) {
    int address = 0;
    int expected = 0x00000000;
    ssd->read(address);
    
    EXPECT_THAT(readResult(), testing::Eq(expected));
}

TEST_F(SSDFIxture, SsdRead99) {
    int address = 99;
    int expected = 0x00000000;
    ssd->read(address);
    EXPECT_THAT(readResult(), testing::Eq(expected));
}

TEST_F(SSDFIxture, SsdRead100) {
    int address = 100;
    int expected = 0x00000000;
    EXPECT_THROW({
         ssd->read(address);
        }, ssd_exception);
}

TEST_F(SSDFIxture, SsdBrokenFile) {
    ofstream testFile(NAND);
    for (int i = 0; i < 1; i++) {
        testFile << "0" << endl;
    }
    testFile.close();

    EXPECT_THROW({
         ssd->read(0);
        }, ssd_exception);
}

TEST_F(SSDFIxture, CommandInvokerEmptyCommand) {
    int argc = 1;
    char* argv[] = { "ssd.exe" };

    EXPECT_THROW({
        invoker.executeCommands(argc, argv);
        }, ssd_exception);
}

TEST_F(SSDFIxture, CommandInvokerEmptyArgument) {
    int argc = 1;
    char* argv[] = { "ssd.exe", "W"};

    EXPECT_THROW({
        invoker.executeCommands(argc, argv);
        }, ssd_exception);
}

TEST_F(SSDFIxture, CommandInvokerLessArgument) {
    int argc = 1;
    char* argv[] = { "ssd.exe", "W", "0"};

    EXPECT_THROW({
        invoker.executeCommands(argc, argv);
        }, ssd_exception);
}

TEST_F(SSDFIxture, CommandInvokerWrite0) {
    int ret = makeWriteCmd("0", "0xdeadbeef");

    EXPECT_THAT(ret, testing::Eq(ICommand::COMMAND_VALIDATION_SUCCESS));
}

TEST_F(SSDFIxture, CommandInvokerRead0) {
    int expectedData = 0x00000000;

    makeReadCmd("0");
    EXPECT_THAT(readResult(), testing::Eq(expectedData));
}

TEST_F(SSDFIxture, CommandInvokerWrite100) {
    EXPECT_THROW({
         makeWriteCmd("100", "0xdeadbeef");
        }, ssd_exception);
}

TEST_F(SSDFIxture, CommandInvokerRead100) {

    EXPECT_THROW({
          makeReadCmd("100");
        }, ssd_exception);
}

TEST_F(SSDFIxture, CommandInvokerWriteRead0Verify) {
    makeWriteCmd("0", "0xdeadbeef");
    makeReadCmd("0");

    int expectedData = 0xdeadbeef;
    EXPECT_THAT(readResult(), testing::Eq(expectedData));
}

TEST_F(SSDFIxture, CommandInvokerWriteRead50Verify) {
    makeWriteCmd("50", "0xdeadbeef");
    makeReadCmd("50");

    int expectedData = 0xdeadbeef;
    EXPECT_THAT(readResult(), testing::Eq(expectedData));
}


TEST_F(SSDFIxture, CommandInvokerWriteInvalidData) {
    EXPECT_THROW({
         makeWriteCmd("0", "0xzzzzzzzz");
        }, ssd_exception);
}

TEST_F(SSDFIxture, CommandInvokerWriteInvalidDataLong) {
    EXPECT_THROW({
         makeWriteCmd("0", "0xdeadbeefffff");
        }, ssd_exception);
}

TEST_F(SSDFIxture, CommandInvokerReadInvalidAddressFormat0x0) {
    int argc = 3;
    char* argv[] = { "ssd.exe", "R", "0x0"};
    EXPECT_THROW({
         invoker.executeCommands(argc, argv);
        }, ssd_exception);
}

TEST_F(SSDFIxture, CommandInvokerWriteReadEraseReadVerify) {
    makeWriteCmd("0", "0xdeadbeef");
    makeReadCmd("0");

    int expectedData = 0xdeadbeef;
    EXPECT_THAT(readResult(), testing::Eq(expectedData));

    makeEraseCmd("0", "1");
    makeReadCmd("0");

    expectedData = 0x00000000;
    EXPECT_THAT(readResult(), testing::Eq(expectedData));
}

TEST_F(SSDFIxture, CommandInvokerWriteAndErase) {
    makeWriteCmd("10", "0xdeadbeef");
    makeWriteCmd("10", "0x00000001");
    makeReadCmd("10");

    int expectedData = 0x00000001;
    EXPECT_THAT(readResult(), testing::Eq(expectedData));
 
    makeEraseCmd("9", "3");
    makeReadCmd("10");
    expectedData = 0x00000000;
    EXPECT_THAT(readResult(), testing::Eq(expectedData));
}