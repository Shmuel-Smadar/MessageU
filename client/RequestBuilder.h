#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include "CurrentUser.h"
#include "Utils.h"
#include "UserInfoList.h"

namespace RequestType {
	constexpr uint16_t Registration = 600;
	constexpr uint16_t ClientList = 601;
	constexpr uint16_t PublicKey = 602;
};


class RequestBuilder {
private:
	uint8_t clientVersion = 1;

	
	std::vector<uint8_t> buildRequestHeaders(const CurrentUser& currentUser);

public:
	std::vector<uint8_t> buildRegistrationRequest(const CurrentUser& currentUser, const std::string& publicKey);
	std::vector<uint8_t> buildClientsListRequest(CurrentUser& currentUser);
	std::vector<uint8_t> buildPublicKeyRequest(CurrentUser& currentUser, UserInfo* userInfo);
};