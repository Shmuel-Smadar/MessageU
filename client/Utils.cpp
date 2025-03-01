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

    uint16_t Utils::parseUint16(const std::vector<uint8_t>& data) {
        return static_cast<uint16_t>(data[0]) |
            (static_cast<uint16_t>(data[1]) << 8);
    }

    uint32_t Utils::parseUint32(const std::vector<uint8_t>& bytes) {
        return static_cast<uint32_t>(bytes[0])
            | (static_cast<uint32_t>(bytes[1]) << 8)
            | (static_cast<uint32_t>(bytes[2]) << 16)
            | (static_cast<uint32_t>(bytes[3]) << 24);
    }
    // appends the type Message into a given buffer
    void Utils::appendMessage(std::vector<uint8_t>& buffer, const Message& message) {
        appendToBuffer(buffer, hexStringToBytes(message.getSenderClientId()));
        buffer.push_back(message.getMessageType());
        appendUint32(buffer, static_cast<uint32_t>(message.getContent().size()));
        appendString(buffer, message.getContent());
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
        if (hex.length() % 2 != 0) 
            throw std::invalid_argument("Hex string must have an even length.");
        for (size_t i = 0; i < hex.length(); i += 2) {
            std::string byteString = hex.substr(i, 2);
            char byte = static_cast<char>(strtol(byteString.c_str(), nullptr, 16));
            bytes += byte;
        }
        return bytes;
    }

    /* a function that cast a string into an int, supporting negative or positive leading sign. */
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
            throw std::invalid_argument("Sign character only.");

        int result = 0;

        for (int i = start; i < str.size(); ++i)
        {
            if (str[i] < '0' || str[i] > '9')
                throw std::invalid_argument("Invalid Integer");

            result = result * 10 - (str[i] - '0');
        }
        return negate ? result : -result;
    }
}
// a function that trims the string after this first '\0'
std::string Utils::trimAfterNull(const std::string& str) {
    size_t pos = str.find('\0');
    if (pos != std::string::npos)
        return str.substr(0, pos);
    return str;
}

uint8_t hexCharToByte(char c) {
    if (std::isdigit(static_cast<unsigned char>(c))) {
        return c - '0';
    }
    else if (std::isxdigit(static_cast<unsigned char>(c))) {
        return static_cast<uint8_t>(std::toupper(c)) - 'A' + 10;
    }
    else {
        throw std::invalid_argument("Invalid hex character encountered.");
    }
}


std::vector<uint8_t> Utils::hexStringToBytes(const std::string& hex) {
    if (hex.length() % 2 != 0) {
        throw std::invalid_argument("Hex string must have an even length.");
    }

    std::vector<uint8_t> bytes;
    bytes.reserve(hex.length() / 2);
    for (size_t i = 0; i < hex.length(); i += 2) {
        uint8_t high = hexCharToByte(hex[i]);
        uint8_t low = hexCharToByte(hex[i + 1]);
        bytes.push_back((high << 4) | low);
    }
    return bytes;
}

void Utils::appendToBuffer(std::vector<uint8_t>& buffer, const std::vector<uint8_t>& source) {
    buffer.insert(buffer.end(), source.begin(), source.end());
}