#include "Utils.h"


namespace Utils {

    std::string bytesToHex(const std::string& bytes) {
        std::ostringstream oss;
        for (unsigned char byte : bytes) {
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
        }
        return oss.str();
    }

    std::string hexToBytes(const std::string& hex) {
        std::string bytes;
        if (hex.length() % 2 != 0) return "";
        for (size_t i = 0; i < hex.length(); i += 2) {
            std::string byteString = hex.substr(i, 2);
            char byte = static_cast<char>(strtol(byteString.c_str(), nullptr, 16));
            bytes += byte;
        }
        return bytes;
    }

}
