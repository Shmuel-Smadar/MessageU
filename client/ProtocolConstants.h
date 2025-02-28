/*
 * File Name: ProtocolConstants.h
 *
 * This file defines protocol-related constants, including request types,
 * message types, server response codes, byte sizes for the defined components,
 * and a struct for the header.
 */


#pragma once
#include <cstdint>

namespace RequestType {
	constexpr uint16_t Registration = 600;
	constexpr uint16_t ClientList = 601;
	constexpr uint16_t PublicKey = 602;
	constexpr uint16_t SendMessage = 603;
	constexpr uint16_t AwaitingMessages = 604;
}
namespace Version {
	constexpr uint8_t ClientVersion = 1;
}
namespace ServerCodes {
	constexpr uint16_t Error = 9000;
}
namespace MessageType {
	constexpr uint8_t SymmetricKeyRequest = 1;
	constexpr uint8_t SymmetricKeySent = 2;
	constexpr uint8_t TextMessageSent = 3;
}

namespace ProtocolByteSizes {
	constexpr size_t Version = 1;
	constexpr size_t Code = 2;
	constexpr size_t PayloadSize = 4;
	constexpr size_t Header = 7; // Version + Code + PayloadSize = 1 + 2 + 4 = 7
	constexpr size_t ClientName = 255;
	constexpr size_t ClientId = 16;
	constexpr size_t PublicKey = 160;
	constexpr size_t MessageType = 1;
	constexpr size_t MessageLength = 4;
	constexpr size_t MessageHeaderRequest = 21; //ClientId + MessageType + MessageLength = 16 + 1 + 4 = 21
	constexpr size_t MessageId = 4;
	constexpr size_t MessageHeaderResponse = 25; //ClientId + MessageType + MessageLength + MessageId = 16 + 1 + 4 + 4 = 25
}

struct ResponseHeader {
	uint8_t version;
	uint16_t code;
	uint32_t payloadSize;

	ResponseHeader(uint8_t ver = 0, uint16_t c = 0, uint32_t size = 0)
		: version(ver), code(c), payloadSize(size) {}
};