#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include "CurrentUser.h"
#include "Utils.h"
#include "UserInfo.h"
#include "UserInfoList.h"
#include "EncryptionManager.h"

namespace ProtocolSizes {
	constexpr size_t Header = 7;
	constexpr size_t ClientName = 255;
	constexpr size_t ClientId = 16;
};

struct ResponseHeader {
	uint8_t version;
	uint16_t code;
	uint32_t payloadSize;

	ResponseHeader(uint8_t ver = 0, uint16_t c = 0, uint32_t size = 0)
		: version(ver), code(c), payloadSize(size) {}
};

class ResponseParser {
private:
	uint8_t clientVersion = 1;

	std::unique_ptr<ResponseHeader> parseResponseHeaders(const std::vector<uint8_t>& data);

public:
	bool parseRegistrationResponse(const std::vector<uint8_t>& data, CurrentUser& currentUser);
	bool parseClientsListResponse(const std::vector<uint8_t>& data, UserInfoList& userInfoList);
	bool parsePublicKeyResponse(const std::vector<uint8_t>& data, const UserInfo& userInfo, EncryptionManager& encryptionManager);
	bool parseAwaitingMessagesResponse(const std::vector<uint8_t>& data, UserInfoList& userInfoList, EncryptionManager& encryptionManager);
	bool parseSymmetricKeyRequestResponse(const std::vector<uint8_t>& data, const UserInfo* userInfo);
};