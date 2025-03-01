#include "RequestBuilder.h"


std::vector<uint8_t> RequestBuilder::buildRegistrationRequest(const CurrentUser& currentUser, const std::string& publicKey) {
    std::vector<uint8_t> buffer = buildRequestHeaders(currentUser);
    Utils::appendString(buffer, "0000000000000000"); //add empty clientID since we don't have one yet
    Utils::appendUint16(buffer, static_cast<uint16_t>(RequestType::Registration));

    std::vector<uint8_t> payload;
    Utils::appendString(payload, currentUser.getName());
    size_t paddingSize = ProtocolByteSizes::ClientName - currentUser.getName().length();
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

std::vector<uint8_t> RequestBuilder::buildPublicKeyRequest(CurrentUser& currentUser, UserInfo& userInfo) {
    std::vector<uint8_t> buffer = buildRequestHeaders(currentUser);
    Utils::appendUint16(buffer, RequestType::PublicKey);
    std::vector<uint8_t> payload;
    Utils::appendToBuffer(payload, Utils::hexStringToBytes(userInfo.getClientID()));
    Utils::appendUint32(buffer, static_cast<uint32_t>(payload.size()));
    buffer.insert(buffer.end(), payload.begin(), payload.end());
    return buffer;
}

std::vector<uint8_t> RequestBuilder::buildWaitingMessagesRequest(CurrentUser& currentUser) {
    std::vector<uint8_t> buffer = buildRequestHeaders(currentUser);
    Utils::appendUint16(buffer, RequestType::AwaitingMessages);
    std::vector<uint8_t> payload;
    Utils::appendUint32(buffer, static_cast<uint32_t>(payload.size()));
    buffer.insert(buffer.end(), payload.begin(), payload.end());
    return buffer;
}

std::vector<uint8_t> RequestBuilder::buildSymmetricKeyRequest(CurrentUser& currentUser, UserInfo& userInfo, EncryptionManager& encryptionManager) {
    std::vector<uint8_t> buffer = buildRequestHeaders(currentUser);
    Utils::appendUint16(buffer, RequestType::SendMessage);
    std::string content = encryptionManager.encryptWithPublicKey(userInfo.getClientID(), ""); // content will be a cipher for an empty string
    Message message(userInfo, MessageType::SymmetricKeyRequest, content);
    Utils::appendUint32(buffer, static_cast<uint32_t>(message.getContent().size() + ProtocolByteSizes::MessageHeaderRequest));
    Utils::appendMessage(buffer, message);
    return buffer;
}

std::vector<uint8_t> RequestBuilder::buildSendSymmetricKey(CurrentUser& currentUser, UserInfo& userInfo, EncryptionManager& encryptionManager) {
    std::vector<uint8_t> buffer = buildRequestHeaders(currentUser);
    Utils::appendUint16(buffer, RequestType::SendMessage);
    encryptionManager.generateSymmetricKey(userInfo.getClientID());
    std::string symmetricKey = encryptionManager.getSymmetricKey(userInfo.getClientID());
    Message message(userInfo, MessageType::SymmetricKeySent, encryptionManager.encryptWithPublicKey(userInfo.getClientID(), symmetricKey));
    Utils::appendUint32(buffer, static_cast<uint32_t>(message.getContent().size() + ProtocolByteSizes::MessageHeaderRequest));
    Utils::appendMessage(buffer, message);
    return buffer;
}
std::vector<uint8_t> RequestBuilder::buildTextMessageRequest(CurrentUser& currentUser, UserInfo& userInfo, std::string& textMessage, EncryptionManager& encryptionManager) {
    std::vector<uint8_t> buffer = buildRequestHeaders(currentUser);
    Utils::appendUint16(buffer, RequestType::SendMessage);
    std::string cipher = encryptionManager.encryptWithSymmetricKey(userInfo.getClientID(), textMessage);
    Message message(userInfo, MessageType::TextMessageSent, cipher);
    Utils::appendUint32(buffer, static_cast<uint32_t>(message.getContent().size() + ProtocolByteSizes::MessageHeaderRequest));
    Utils::appendMessage(buffer, message);
    return buffer;
}

std::vector<uint8_t> RequestBuilder::buildRequestHeaders(const CurrentUser& currentUser) {
    std::vector<uint8_t> buffer = Utils::hexStringToBytes(currentUser.getClientID());
    buffer.push_back(Version::ClientVersion);
    return buffer;
}




