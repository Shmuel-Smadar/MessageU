#pragma once
#include <cstdint>

namespace RequestType {
	constexpr uint16_t Registration = 600;
	constexpr uint16_t ClientList = 601;
	constexpr uint16_t PublicKey = 602;
	constexpr uint16_t SendMessage = 603;
	constexpr uint16_t AwaitingMessages = 604;
}
namespace ServerCodes {
	uint16_t Error = 9000;
}
namespace MessageType {
	constexpr uint8_t SymmetricKeyRequest = 1;
	constexpr uint8_t SymmetricKeySent = 2;
	constexpr uint8_t TextMessageSent = 3;
}

namespace ProtocolSizes {
	constexpr size_t Header = 7;
	constexpr size_t ClientName = 255;
	constexpr size_t ClientId = 16;
}

struct ResponseHeader {
	uint8_t version;
	uint16_t code;
	uint32_t payloadSize;

	ResponseHeader(uint8_t ver = 0, uint16_t c = 0, uint32_t size = 0)
		: version(ver), code(c), payloadSize(size) {}
};