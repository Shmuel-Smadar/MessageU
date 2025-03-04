#include "ResponseParser.h"


void ResponseParser::parseRegistrationResponse(const std::vector<uint8_t>& data, CurrentUser& currentUser) {

	std::unique_ptr<ResponseHeader> header = parseResponseHeaders(data);
	size_t pos = ProtocolByteSizes::Header;
	std::string clientID(data.begin() + pos, data.begin() + pos + ProtocolByteSizes::ClientId);
	currentUser.setClientID(Utils::bytesToHex(clientID));
	currentUser.setRegistered(true);
}

void ResponseParser::parseClientsListResponse(const std::vector<uint8_t>& data, UserInfoList& userInfoList) {
	std::unique_ptr<ResponseHeader> header = parseResponseHeaders(data);
	size_t pos = ProtocolByteSizes::Header;
	size_t userDataLength = ProtocolByteSizes::ClientId + ProtocolByteSizes::ClientName;
	while (pos + userDataLength <= data.size()) {
		std::string clientId(data.begin() + pos, data.begin() + pos + ProtocolByteSizes::ClientId);
		pos += ProtocolByteSizes::ClientId;

		std::string clientName(data.begin() + pos, data.begin() + pos + ProtocolByteSizes::ClientName);
		pos += ProtocolByteSizes::ClientName;
		userInfoList.addUser(Utils::bytesToHex(clientId), Utils::trimAfterNull(clientName));
	}
}

void ResponseParser::parsePublicKeyResponse(const std::vector<uint8_t>& data, UserInfo& userInfo, EncryptionManager& encryptionManager) {
	std::unique_ptr<ResponseHeader> header = parseResponseHeaders(data);
	size_t pos = ProtocolByteSizes::Header;
	std::string publicKey = std::string(data.begin() + pos, data.begin() + pos + ProtocolByteSizes::PublicKey);
	encryptionManager.storePublicKey(userInfo.getClientID(), publicKey);
	userInfo.publicKeyReceived();
}

std::vector<Message> ResponseParser::parseAwaitingMessagesResponse(const std::vector<uint8_t>& data, UserInfoList& userInfoList, EncryptionManager& encryptionManager) {
	std::unique_ptr<ResponseHeader> header = parseResponseHeaders(data);
	size_t pos = ProtocolByteSizes::Header;
	std::vector<Message> messages;
	while (pos + ProtocolByteSizes::MessageHeaderResponse <= data.size()) {
		//read clientId
		std::string fromClient(data.begin() + pos, data.begin() + pos + ProtocolByteSizes::ClientId);
		pos += ProtocolByteSizes::ClientId;
		//read messagedId
		std::vector<uint8_t> messageIdBytes(data.begin() + pos, data.begin() + pos + ProtocolByteSizes::MessageId);
		uint32_t messageID = Utils::parseUint32(messageIdBytes);
		pos += ProtocolByteSizes::MessageId;
		//read messaged type
		uint8_t messageType = data[pos];
		pos += ProtocolByteSizes::MessageType;
		//read message length
		std::vector<uint8_t> messageLengthBytes(data.begin() + pos, data.begin() + pos + ProtocolByteSizes::MessageLength);
		uint32_t messageLength = Utils::parseUint32(messageLengthBytes);
		pos += ProtocolByteSizes::MessageLength;
		//read message content based on the length
		std::string content(reinterpret_cast<const char*>(&data[pos]), messageLength);
		pos += messageLength;
		//create a message and send it to function for parsing, finally add it to a vector of messages.
		Message message(userInfoList.getUserByID(Utils::bytesToHex(fromClient)), messageType, content);
		parseMessage(message, encryptionManager);
		messages.emplace_back(message);
	}
	return messages;
}

void ResponseParser::parseMessage(Message& message, EncryptionManager& encryptionManager) {
	UserInfo userInfo = message.getUser();
	std::string senderClientId = message.getSenderClientId();
	if (message.getMessageType() == MessageType::SymmetricKeyRequest) {
		userInfo.otherUserRequestedSymmetricKey();
	}
	else if (message.getMessageType() == MessageType::SymmetricKeySent) {
		encryptionManager.storeSymmetricKey(senderClientId, encryptionManager.decryptWithPrivateKey(message.getContent()));
		userInfo.publicKeyReceived();
	}
	else if (message.getMessageType() == MessageType::TextMessageSent) {
		message.setContent(encryptionManager.decryptWithSymmetricKey(senderClientId, message.getContent()));
	}
}


//TODO: func name needs changing...
void ResponseParser::parseSymmetricKeyRequestResponse(const std::vector<uint8_t>& data, const UserInfo& userInfo) {
	std::unique_ptr<ResponseHeader> header = parseResponseHeaders(data);
	size_t pos = ProtocolByteSizes::Header;
	std::string requestedUserClientId = Utils::bytesToHex(std::string(data.begin() + pos, data.begin() + pos + ProtocolByteSizes::ClientId));
	pos += ProtocolByteSizes::ClientId;
	uint32_t messageID = Utils::parseUint32(std::vector<uint8_t>(data.begin() + pos, data.begin() + pos + ProtocolByteSizes::MessageId));
}
void ResponseParser::parseTextMessageResponse(const std::vector<uint8_t>& data, const UserInfo& userInfo) {
	std::unique_ptr<ResponseHeader> header = parseResponseHeaders(data);
	size_t pos = ProtocolByteSizes::Header;
	std::string requestedUserClientId = Utils::bytesToHex(std::string(data.begin() + pos, data.begin() + pos + ProtocolByteSizes::ClientId));
	uint32_t messageID = Utils::parseUint32(std::vector<uint8_t>(data.begin() + pos, data.begin() + pos + ProtocolByteSizes::MessageId));
}

std::unique_ptr<ResponseHeader> ResponseParser::parseResponseHeaders(const std::vector<uint8_t>& data) {
	if (data.size() < ProtocolByteSizes::Header) {
		throw std::runtime_error("Server response does not meet header sizes defined in the protocol.");
	}

	uint8_t version = data[0];
	uint16_t code = Utils::parseUint16(
		std::vector<uint8_t>(data.begin() + ProtocolByteSizes::Version,
			data.begin() + ProtocolByteSizes::Version + ProtocolByteSizes::Code)
	);
	uint32_t payloadSize = Utils::parseUint32(
		std::vector<uint8_t>(data.begin() + ProtocolByteSizes::Version + ProtocolByteSizes::Code,
			data.begin() + ProtocolByteSizes::Version + ProtocolByteSizes::Code + ProtocolByteSizes::PayloadSize)
	);

	if (code == ServerCodes::Error) {
		throw std::runtime_error("Server responded with an error.");
	}
	return std::make_unique<ResponseHeader>(version, code, payloadSize);
}

