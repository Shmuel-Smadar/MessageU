#include "ProtocolHandler.h"

void appendUint16(std::vector<uint8_t>& buffer, uint16_t value) {
    buffer.push_back(value & 0xFF);
    buffer.push_back((value >> 8) & 0xFF);
}
void appendString(std::vector<uint8_t>& buffer, const std::string& str) {
    buffer.insert(buffer.end(), str.begin(), str.end());
}
void appendUint32(std::vector<uint8_t>& buffer, uint32_t value) {
    buffer.push_back(value & 0xFF);
    buffer.push_back((value >> 8) & 0xFF);
    buffer.push_back((value >> 16) & 0xFF);
    buffer.push_back((value >> 24) & 0xFF);
}

/*TODO: 
1. make sure the user cant input name that is longer than 255 bytes.
2. see how you can push the name into the buffer with some garbage values maybe if the name is shorter than 255 bytes.*/
std::vector<uint8_t> ProtocolHandler::buildRegistrationRequest(const CurrentUser& currentUser, const std::string& publicKey) {
    std::vector<uint8_t> buffer = buildRequestHeaders(currentUser);
    
    appendUint16(buffer, static_cast<uint16_t>(RequestType::Registration));

    std::vector<uint8_t> payload;

    appendString(payload, currentUser.getName());

    size_t paddingSize = 255 - currentUser.getName().length();
    std::vector<uint8_t> padding(paddingSize, '\0');

    payload.insert(payload.end(), padding.begin(), padding.end());
    appendString(payload, publicKey);

    appendUint32(buffer, static_cast<uint32_t>(payload.size()));

    buffer.insert(buffer.end(), payload.begin(), payload.end());

    return buffer;
}

std::vector<uint8_t> ProtocolHandler::buildRequestHeaders(const CurrentUser &currentUser) {
    std::vector<uint8_t> buffer;
    buffer.push_back(clientVersion);
    return buffer;
}

bool ProtocolHandler::parseRegistrationResponse(const std::vector<uint8_t>& data, CurrentUser& currentUser) {
   
    if (parseResponseHeaders(data)) {
        currentUser.setClientID(std::string(data.begin() + 7, data.begin() + 23));
        currentUser.setRegistered(true);
        return true;
    }
    return false;
}
bool ProtocolHandler::parseResponseHeaders(const std::vector<uint8_t>& data) {
    ResponseHeader header;
    header.version = static_cast<uint8_t>(data[0]);


    header.code = static_cast<uint16_t>(data[1]) |
        (static_cast<uint16_t>(data[2]) << 8);

    header.payloadSize = static_cast<uint32_t>(data[3]) |
        (static_cast<uint32_t>(data[4]) << 8) |
        (static_cast<uint32_t>(data[5]) << 16) |
        (static_cast<uint32_t>(data[6]) << 24);
    if (header.code == 9000)
        return false;
    return true;
}