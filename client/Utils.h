#pragma once
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <vector>

namespace Utils {
    std::string bytesToHex(const std::string& bytes);
    std::string hexToBytes(const std::string& hex);
    int stringToInt(const std::string& str);
    void appendUint16(std::vector<uint8_t>& buffer, uint16_t value);
    void appendString(std::vector<uint8_t>& buffer, const std::string& str);
    void appendUint32(std::vector<uint8_t>& buffer, uint32_t value);
}
