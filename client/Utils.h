/*
 * File Name: Utils.h
 *
 * This file is responsible for providing utility functions for byte operations,
 * buffers management and various conversions.
 */

#pragma once
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <vector>
#include "Message.h"

namespace Utils {
    std::string bytesToHex(const std::string& bytes);
    std::string hexToBytes(const std::string& hex);
    int stringToInt(const std::string& str);
    void appendUint16(std::vector<uint8_t>& buffer, uint16_t value);
    void appendString(std::vector<uint8_t>& buffer, const std::string& str);
    void appendUint32(std::vector<uint8_t>& buffer, uint32_t value);
    uint32_t parseUint32(const std::vector<uint8_t>& bytes);
    uint16_t parseUint16(const std::vector<uint8_t>& data);
    void appendMessage(std::vector<uint8_t>& buffer, const Message& message);
    std::string trimAfterNull(const std::string& str);
    std::vector<uint8_t> hexStringToBytes(const std::string& hex);
    void appendToBuffer(std::vector<uint8_t>& buffer, const std::vector<uint8_t>& source);
}
