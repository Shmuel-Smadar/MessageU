#pragma once
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <string>

namespace Utils {
    std::string bytesToHex(const std::string& bytes);
    std::string hexToBytes(const std::string& hex);
    int stringToInt(const std::string& str);
}
