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
1. create seperate function for pushing the clientID and client version into the buffer.
2. make sure the user cant input name that is longer than 255 bytes.
3. see how you can push the name into the buffer with some garbage values maybe if the name is shorter than 255 bytes.*/
std::vector<uint8_t> ProtocolHandler::buildRegistrationRequest(const std::string& name, const std::string& publicKey) {
    std::vector<uint8_t> buffer;
    buffer.push_back(clientVersion);
    appendUint16(buffer, static_cast<uint16_t>(RequestType::Registration));

    std::vector<uint8_t> payload;
    appendString(payload, name + '\0');
    appendString(payload, publicKey);

    appendUint32(buffer, static_cast<uint32_t>(payload.size()));

    buffer.insert(buffer.end(), payload.begin(), payload.end());

    return buffer;
}