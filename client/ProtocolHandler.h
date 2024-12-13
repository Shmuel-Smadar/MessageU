#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include "CurrentUser.h";
#include "Utils.h";
enum class RequestType : uint16_t {
    Registration = 600,

};

struct ResponseHeader {
    uint8_t version;      // 1 byte
    uint16_t code;        // 2 bytes
    uint32_t payloadSize; // 4 bytes

    ResponseHeader(uint8_t ver = 0, uint16_t c = 0, uint32_t size = 0)
        : version(ver), code(c), payloadSize(size) {}
};

class ProtocolHandler {
private:
	uint8_t clientVersion = 1;
    bool parseResponseHeaders(const std::vector<uint8_t>& data);

public:
	std::vector<uint8_t> buildRegistrationRequest(const CurrentUser& currentUser, const std::string& publicKey);
    std::vector<uint8_t> buildRequestHeaders(const CurrentUser &currentUser);
    bool parseRegistrationResponse(const std::vector<uint8_t>& data, CurrentUser& currentUser);
   
};