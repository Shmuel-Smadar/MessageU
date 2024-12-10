#pragma once

#include <vector>
#include <string>
#include <cstdint>


enum class RequestType : uint16_t {
    Registration = 610,
    SymmetricKeyRequest = 2,
    SymmetricKeySend = 3,
};


class ProtocolHandler {
private:
	uint8_t clientVersion = 1;

public:
	std::vector<uint8_t> buildRegistrationRequest(const std::string& name, const std::string& publicKey);
};