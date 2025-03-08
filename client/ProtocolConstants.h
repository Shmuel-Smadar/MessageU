/*
 * File Name: ProtocolConstants.h
 *
 * This file defines protocol-related constants, including request types,
 * message types, server response codes, byte sizes for the defined components,
 * and a struct for the header.
 */


#pragma once
#include <cstdint>

namespace MenuOptions {
	constexpr uint8_t Register = 110;
	constexpr uint8_t RequestClientsList = 120;
	constexpr uint8_t RequestPublicKey = 130;
	constexpr uint8_t RequestWaitingMessages = 140;
	constexpr uint8_t SendTextMessage = 150;
	constexpr uint8_t RequestSymmetricKey = 151;
	constexpr uint8_t SendSymmetricKey = 152;
	constexpr uint8_t Exit = 0;
}

namespace ClientCodes {
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
	constexpr uint16_t RegistrationSuccessful = 2100;
	constexpr uint16_t ReturnedClientList = 2101;
	constexpr uint16_t ReturnedPublicKey = 2102;
	constexpr uint16_t MessageSent = 2103;
	constexpr uint16_t ReturnedAwaitingMessages = 2104;
	constexpr uint16_t Error = 9000;
}

enum class SupportedServerVersions : uint8_t {
	V1 = 1,
	V2 = 2
};
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
	constexpr size_t MessageMaxSize = 32;
}

struct ResponseHeader {
	uint8_t version;
	uint16_t code;
	uint32_t payloadSize;

	ResponseHeader(uint8_t ver = 0, uint16_t c = 0, uint32_t size = 0)
		: version(ver), code(c), payloadSize(size) {}
};