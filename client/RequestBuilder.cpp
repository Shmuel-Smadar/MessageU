#include "RequestBuilder.h"


std::vector<uint8_t> RequestBuilder::buildRegistrationRequest(const CurrentUser& currentUser, const std::string& publicKey) {
    std::vector<uint8_t> buffer = buildRequestHeaders(currentUser);
    Utils::appendString(buffer, "0000000000000000"); //Empty clientID
    Utils::appendUint16(buffer, static_cast<uint16_t>(RequestType::Registration));

    std::vector<uint8_t> payload;
    Utils::appendString(payload, currentUser.getName());
    size_t paddingSize = 255 - currentUser.getName().length();
    std::vector<uint8_t> padding(paddingSize, '\0');
    payload.insert(payload.end(), padding.begin(), padding.end());
    Utils::appendString(payload, publicKey);

    Utils::appendUint32(buffer, static_cast<uint32_t>(payload.size()));
    buffer.insert(buffer.end(), payload.begin(), payload.end());
    return buffer;
}

std::vector<uint8_t> RequestBuilder::buildClientsListRequest(CurrentUser& currentUser) {
    std::vector<uint8_t> buffer = buildRequestHeaders(currentUser);
    Utils::appendUint16(buffer, RequestType::ClientList);

    std::vector<uint8_t> payload;
    Utils::appendUint32(buffer, static_cast<uint32_t>(payload.size()));
    buffer.insert(buffer.end(), payload.begin(), payload.end());
    return buffer;
}


std::vector<uint8_t> RequestBuilder::buildPublicKeyRequest(CurrentUser& currentUser, UserInfo* userInfo) {
    std::vector<uint8_t> buffer = buildRequestHeaders(currentUser);
    Utils::appendUint16(buffer, RequestType::PublicKey);
    std::vector<uint8_t> payload;
    Utils::appendString(payload, userInfo->getClientID());
    Utils::appendUint32(buffer, static_cast<uint32_t>(payload.size()));
    buffer.insert(buffer.end(), payload.begin(), payload.end());
    return buffer;
}

std::vector<uint8_t> RequestBuilder::buildWaitingMessagesRequest(CurrentUser& currentUser) {
    std::vector<uint8_t> buffer = buildRequestHeaders(currentUser);
    Utils::appendUint16(buffer, RequestType::WaitingMessages);
    std::vector<uint8_t> payload;
    Utils::appendUint32(buffer, static_cast<uint32_t>(payload.size()));
    buffer.insert(buffer.end(), payload.begin(), payload.end());
    return buffer;
}

std::vector<uint8_t> RequestBuilder::buildSymmetricKeyRequest(CurrentUser& currentUser, UserInfo* userInfo, EncryptionManager& encryptionManager) {
    std::vector<uint8_t> buffer = buildRequestHeaders(currentUser);
    Utils::appendUint16(buffer, RequestType::SendMessage);
    Message message(userInfo->getClientID(), MessageType::SymmetricKeyRequest, "");
    Utils::appendMessage(buffer, message);
    return buffer;
}


std::vector<uint8_t> RequestBuilder::buildRequestHeaders(const CurrentUser& currentUser) {
    std::vector<uint8_t> buffer = Utils::hexStringToBytes(currentUser.getClientID());
    buffer.push_back(clientVersion);
    return buffer;
}




