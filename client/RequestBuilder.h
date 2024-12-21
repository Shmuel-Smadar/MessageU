#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include "Message.h"
#include "CurrentUser.h"
#include "Utils.h"
#include "UserInfoList.h"
#include "EncryptionManager.h"

namespace RequestType {
	constexpr uint16_t Registration = 600;
	constexpr uint16_t ClientList = 601;
	constexpr uint16_t PublicKey = 602;
	constexpr uint16_t SendMessage = 603;
	constexpr uint16_t AwaitingMessages = 604;
};

namespace MessageType {
	constexpr uint8_t SymmetricKeyRequest = 1;
	constexpr uint8_t SymmetricKeySent = 2;
	constexpr uint8_t TextMessageSent = 3;
}
class RequestBuilder {
private:
	uint8_t clientVersion = 1;

	
	std::vector<uint8_t> buildRequestHeaders(const CurrentUser& currentUser);

public:
	std::vector<uint8_t> buildRegistrationRequest(const CurrentUser& currentUser, const std::string& publicKey);
	std::vector<uint8_t> buildClientsListRequest(CurrentUser& currentUser);
	std::vector<uint8_t> buildPublicKeyRequest(CurrentUser& currentUser, UserInfo* userInfo);
	std::vector<uint8_t> buildWaitingMessagesRequest(CurrentUser& currentUser);
	std::vector<uint8_t> buildSymmetricKeyRequest(CurrentUser& currentUser, UserInfo* userInfo, EncryptionManager& encryptionManager);
};