#include "Utils.h"
#include <sstream>
#include <iomanip>
#include <cstdlib>

namespace Utils {

    void Utils::appendUint16(std::vector<uint8_t>& buffer, uint16_t value) {
        buffer.push_back(value & 0xFF);
        buffer.push_back((value >> 8) & 0xFF);
    }
    void Utils::appendString(std::vector<uint8_t>& buffer, const std::string& str) {
        buffer.insert(buffer.end(), str.begin(), str.end());
    }
    void Utils::appendUint32(std::vector<uint8_t>& buffer, uint32_t value) {
        buffer.push_back(value & 0xFF);
        buffer.push_back((value >> 8) & 0xFF);
        buffer.push_back((value >> 16) & 0xFF);
        buffer.push_back((value >> 24) & 0xFF);
    }

    std::string Utils::bytesToHex(const std::string& bytes) {
        std::ostringstream oss;
        for (unsigned char byte : bytes) {
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
        }
        return oss.str();
    }

    std::string Utils::hexToBytes(const std::string& hex) {
        std::string bytes;
        if (hex.length() % 2 != 0) return ""; // Invalid hex string
        for (size_t i = 0; i < hex.length(); i += 2) {
            std::string byteString = hex.substr(i, 2);
            char byte = static_cast<char>(strtol(byteString.c_str(), nullptr, 16));
            bytes += byte;
        }
        return bytes;
    }

    int Utils::stringToInt(const std::string& str)
    {
        if (str.empty())
            throw std::invalid_argument("empty argument");

        bool negate = false;
        size_t start = 0;

        if (str[0] == '-')
        {
            negate = true;
            start = 1;
        }
        else if (str[0] == '+')
        {
            start = 1;
        }

        if (start == str.size())
            throw std::invalid_argument("sign character only.");

        int result = 0;

        for (int i = start; i < str.size(); ++i)
        {
            if (str[i] < '0' || str[i] > '9')
                throw std::invalid_argument("invalid Integer");

            result = result * 10 - (str[i] - '0');
        }
        return negate ? result : -result;
    }
}
std::string Utils::trimAfterNull(const std::string& str) {
    size_t pos = str.find('\0');
    if (pos != std::string::npos)
        return str.substr(0, pos);
    return str;
}