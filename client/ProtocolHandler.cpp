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

std::vector<uint8_t> ProtocolHandler::buildRegistrationRequest(const CurrentUser& currentUser, const std::string& publicKey) {
    std::vector<uint8_t> buffer = buildRequestHeaders(currentUser);
    appendString(buffer, "0000000000000000"); //Empty clientID
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



bool ProtocolHandler::parseRegistrationResponse(const std::vector<uint8_t>& data, CurrentUser& currentUser) {
   
    if (parseResponseHeaders(data)) {
        currentUser.setClientID(std::string(data.begin() + 7, data.begin() + 23));
        currentUser.setRegistered(true);
        return true;
    }
    return false;
}

std::vector<uint8_t> ProtocolHandler::buildClientsListRequest(CurrentUser& currentUser) {
    std::vector<uint8_t> buffer = buildRequestHeaders(currentUser);
    appendUint16(buffer, RequestType::ClientList);

    std::vector<uint8_t> payload;
    appendString(payload, currentUser.getClientID());

    appendUint32(buffer, static_cast<uint32_t>(payload.size()));
    buffer.insert(buffer.end(), payload.begin(), payload.end());
    return buffer;
}

bool ProtocolHandler::parseClientsListResponse(const std::vector<uint8_t>& data, UserInfoList& userInfoList) {
    if (parseResponseHeaders(data)) {
        size_t pos = ProtocolSizes::Header;
        size_t userDataLength = ProtocolSizes::ClientId + ProtocolSizes::ClientName;
        while (pos + userDataLength <= data.size()) {
            std::string clientId(data.begin() + pos, data.begin() + pos + ProtocolSizes::ClientId);
            pos += ProtocolSizes::ClientId;

            std::string clientName(data.begin() + pos, data.begin() + pos + ProtocolSizes::ClientName);
            pos += ProtocolSizes::ClientName;
            userInfoList.addUser(Utils::bytesToHex(clientId), clientName);
        }
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
std::vector<uint8_t> ProtocolHandler::buildRequestHeaders(const CurrentUser& currentUser) {
    std::vector<uint8_t> buffer;
    appendString(buffer, currentUser.getClientID());
    buffer.push_back(clientVersion);
    return buffer;
}