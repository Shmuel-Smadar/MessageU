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
};
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

class ProtocolHandler {
private:
	uint8_t clientVersion = 1;

	bool parseResponseHeaders(const std::vector<uint8_t>& data);
	std::vector<uint8_t> buildRequestHeaders(const CurrentUser& currentUser);

public:
	std::vector<uint8_t> buildRegistrationRequest(const CurrentUser& currentUser, const std::string& publicKey);
	bool parseRegistrationResponse(const std::vector<uint8_t>& data, CurrentUser& currentUser);
	std::vector<uint8_t> buildClientsListRequest(CurrentUser& currentUser);
	bool parseClientsListResponse(const std::vector<uint8_t>& data, UserInfoList& userInfoList);
};