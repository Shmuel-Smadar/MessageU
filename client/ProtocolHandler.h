#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include "CurrentUser.h";
#include "Utils.h";
enum class RequestType : uint16_t {
    Registration = 610,
    SymmetricKeyRequest = 2,
    SymmetricKeySend = 3,
};


class ProtocolHandler {
private:
	uint8_t clientVersion = 1;

public:
	std::vector<uint8_t> buildRegistrationRequest(const CurrentUser& currentUser, const std::string& publicKey);
    std::vector<uint8_t> buildRequestHeaders(const CurrentUser &currentUser);
};