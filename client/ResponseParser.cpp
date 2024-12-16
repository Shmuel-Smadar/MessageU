#include "ResponseParser.h"

namespace ServerCodes {
	uint16_t Error = 9000;
}

bool ResponseParser::parseRegistrationResponse(const std::vector<uint8_t>& data, CurrentUser& currentUser) {

	std::unique_ptr<ResponseHeader> header = parseResponseHeaders(data);
	if (header == nullptr)
		return false;
	currentUser.setClientID(std::string(data.begin() + 7, data.begin() + 23));
	currentUser.setRegistered(true);
	return true;
}

bool ResponseParser::parseClientsListResponse(const std::vector<uint8_t>& data, UserInfoList& userInfoList) {
	std::unique_ptr<ResponseHeader> header = parseResponseHeaders(data);
	if (header == nullptr)
		return false;
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

bool ResponseParser::parsePublicKeyResponse(const std::vector<uint8_t>& data, const UserInfo& userInfo, EncryptionManager& encryptionManager) {
	std::unique_ptr<ResponseHeader> header = parseResponseHeaders(data);
	if (header == nullptr)
		return false;
	std::string publicKey = std::string(data.begin() + 7, data.begin() + 7 + 160);
	encryptionManager.storePublicKey(userInfo.getClientID(), publicKey);
	return true;
}

bool ResponseParser::parseWaitingMessagesResponse(const std::vector<uint8_t>& data, UserInfoList& userInfoList, EncryptionManager& encryptionManager) {
	return true;
}
//TODO: func name needs changing...
bool ResponseParser::parseSymmetricKeyRequestResponse(const std::vector<uint8_t>& data, const UserInfo* userInfo) {
	std::unique_ptr<ResponseHeader> header = parseResponseHeaders(data);
	if (header == nullptr)
		return false;
	std::string requestedUserClientId = std::string(data.begin() + 7, data.begin() + 7 + 16);
	std::cout << requestedUserClientId << std::endl;
	std::string messsageID = std::string(data.begin() + 23, data.begin() + 23 + 4);
}

std::unique_ptr<ResponseHeader> ResponseParser::parseResponseHeaders(const std::vector<uint8_t>& data) {
	if (data.size() < 7) {
		return nullptr;
	}
	std::unique_ptr<ResponseHeader> header(new ResponseHeader());
	header->version = static_cast<uint8_t>(data[0]);
	header->code = static_cast<uint16_t>(data[1]) |
		(static_cast<uint16_t>(data[2]) << 8);
	header->payloadSize = static_cast<uint32_t>(data[3]) |
		(static_cast<uint32_t>(data[4]) << 8) |
		(static_cast<uint32_t>(data[5]) << 16) |
		(static_cast<uint32_t>(data[6]) << 24);
	if (header->code == ServerCodes::Error)
		return nullptr;
	return header;
}
