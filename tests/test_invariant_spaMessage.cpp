#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <cstring>

// Include the production header to access the real structs and functions
#include "spaMessage.h"

class MacAddressBufferTest : public ::testing::TestWithParam<std::vector<uint8_t>> {};

TEST_P(MacAddressBufferTest, BufferNeverExceedsDeclaredLength) {
    // Invariant: macAddress field must never be written beyond its declared size
    // A valid MAC address "ff:ff:ff:ff:ff:ff" is 17 chars + null = 18 bytes max
    std::vector<uint8_t> payload = GetParam();

    SpaMessage spaMsg;
    // Feed the payload bytes as a spa protocol message
    for (uint8_t byte : payload) {
        spaMsg.pushByte(byte);
    }

    // Retrieve the parsed MAC address
    const char* mac = spaMsg.getWiFiModuleConfigurationData().macAddress;
    size_t macLen = strnlen(mac, 256); // read up to 256 to detect overflow

    // The formatted MAC address must never exceed 17 characters (xx:xx:xx:xx:xx:xx)
    EXPECT_LE(macLen, 17u)
        << "macAddress field overflowed: length=" << macLen << " value=" << mac;

    // Must be null-terminated within the declared buffer size
    // Declared buffer size for macAddress should be at least 18 bytes
    EXPECT_LT(macLen, sizeof(spaMsg.getWiFiModuleConfigurationData().macAddress))
        << "macAddress not null-terminated within buffer bounds";
}

INSTANTIATE_TEST_SUITE_P(
    AdversarialInputs,
    MacAddressBufferTest,
    ::testing::Values(
        // Valid input: standard MAC bytes 0xAA:0xBB:0xCC:0xDD:0xEE:0xFF
        std::vector<uint8_t>{0x00, 0x00, 0x00, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF},
        // Boundary: all 0xFF bytes (max hex values, produces "ff:ff:ff:ff:ff:ff")
        std::vector<uint8_t>{0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        // Exploit case: all 0x00 bytes (min values, produces "00:00:00:00:00:00")
        std::vector<uint8_t>{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        // Oversized payload: extra bytes beyond expected frame length
        std::vector<uint8_t>{0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                             0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
    )
);

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}