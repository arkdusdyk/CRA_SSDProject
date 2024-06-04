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
    int result = ssd.read(address);
    EXPECT_THROW({
         ssd.read(address);
        }, std::out_of_range);
}

