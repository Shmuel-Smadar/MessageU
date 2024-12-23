#include "ResponseParser.h"


bool ResponseParser::parseRegistrationResponse(const std::vector<uint8_t>& data, CurrentUser& currentUser) {

	std::unique_ptr<ResponseHeader> header = parseResponseHeaders(data);
	std::string clientID(data.begin() + 7, data.begin() + 23);
	currentUser.setClientID(Utils::bytesToHex(clientID));
	currentUser.setRegistered(true);
	return true;
}

bool ResponseParser::parseClientsListResponse(const std::vector<uint8_t>& data, UserInfoList& userInfoList) {
	std::unique_ptr<ResponseHeader> header = parseResponseHeaders(data);
	size_t pos = ProtocolSizes::Header;
	size_t userDataLength = ProtocolSizes::ClientId + ProtocolSizes::ClientName;
	while (pos + userDataLength <= data.size()) {
		std::string clientId(data.begin() + pos, data.begin() + pos + ProtocolSizes::ClientId);
		pos += ProtocolSizes::ClientId;

		std::string clientName(data.begin() + pos, data.begin() + pos + ProtocolSizes::ClientName);
		pos += ProtocolSizes::ClientName;
		userInfoList.addUser(Utils::bytesToHex(clientId), Utils::trimAfterNull(clientName));
	}
	return true;
}

bool ResponseParser::parsePublicKeyResponse(const std::vector<uint8_t>& data, UserInfo& userInfo, EncryptionManager& encryptionManager) {
	std::unique_ptr<ResponseHeader> header = parseResponseHeaders(data);
	std::string publicKey = std::string(data.begin() + 7, data.begin() + 7 + 160);
	encryptionManager.storePublicKey(userInfo.getClientID(), publicKey);
	userInfo.publicKeyReceived();
	return true;
}

std::vector<Message> ResponseParser::parseAwaitingMessagesResponse(const std::vector<uint8_t>& data, UserInfoList& userInfoList, EncryptionManager& encryptionManager) {
	std::unique_ptr<ResponseHeader> header = parseResponseHeaders(data);
	size_t pos = ProtocolSizes::Header;
	std::vector<Message> messages;
	while (pos + 16 + 4 + 1 + 4 <= data.size()) {
		std::string fromClient(data.begin() + pos, data.begin() + pos + 16);
		pos += 16;
		uint32_t messageID = Utils::parseUint32(std::vector<uint8_t>(data.begin() + pos, data.begin() + pos + 4));
		pos += 4;
		uint8_t messageType = data[pos];
		pos += 1;
		uint32_t contentSize = Utils::parseUint32(std::vector<uint8_t>(data.begin() + pos, data.begin() + pos + 4));
		pos += 4;
		std::string content(reinterpret_cast<const char*>(&data[pos]), contentSize);
		pos += contentSize;
		Message message(userInfoList.getUserByID(Utils::bytesToHex(fromClient)), messageType, content);
		parseMessage(message, encryptionManager);
		messages.emplace_back(message);
	}
	return messages;
}

void ResponseParser::parseMessage(Message message, EncryptionManager& encryptionManager) {
	UserInfo* userInfo = message.getUser();
	std::string senderClientId = message.getSenderClientId();
	if (message.getMessageType() == MessageType::SymmetricKeyRequest) {
		userInfo->otherUserRequestedSymmericKey();
	}
	else if (message.getMessageType() == MessageType::SymmetricKeySent) {
		encryptionManager.storeSymmetricKey(senderClientId, encryptionManager.decryptWithPrivateKey(message.getContent()));
		userInfo->publicKeyReceived();
	}
	else if (message.getMessageType() == MessageType::TextMessageSent) {
		message.setContent(encryptionManager.decryptWithSymmetricKey(senderClientId, message.getContent()));
	}
}


//TODO: func name needs changing...
bool ResponseParser::parseSymmetricKeyRequestResponse(const std::vector<uint8_t>& data, const UserInfo* userInfo) {
	std::unique_ptr<ResponseHeader> header = parseResponseHeaders(data);
	if (header == nullptr)
		return false;
	std::string requestedUserClientId = std::string(data.begin() + 7, data.begin() + 7 + 16);
	uint32_t messageID = Utils::parseUint32(std::vector<uint8_t>(data.begin() + 23, data.begin() + 23 + 4));
}

std::unique_ptr<ResponseHeader> ResponseParser::parseResponseHeaders(const std::vector<uint8_t>& data) {
	std::unique_ptr<ResponseHeader> header(new ResponseHeader());
	header->version = static_cast<uint8_t>(data[0]);
	header->code = static_cast<uint16_t>(data[1]) |
		(static_cast<uint16_t>(data[2]) << 8);
	header->payloadSize = static_cast<uint32_t>(data[3]) |
		(static_cast<uint32_t>(data[4]) << 8) |
		(static_cast<uint32_t>(data[5]) << 16) |
		(static_cast<uint32_t>(data[6]) << 24);
	if (header->code == ServerCodes::Error)
		throw std::runtime_error("Server responded with an error.");
	return header;
}
