/*
 * File Name: ResponseParser.h
 *
 * This file is responsible for parsing the server responses
 * for the defined operations and calling to functions that are responsible for
 * managing the data structures and encryption appropriately.
 */



#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include "CurrentUser.h"
#include "Utils.h"
#include "UserInfo.h"
#include "UserInfoList.h"
#include "EncryptionManager.h"
#include "ProtocolConstants.h"

class ResponseParser {
private:
	std::unique_ptr<ResponseHeader> parseResponseHeaders(const std::vector<uint8_t>& data);

public:
	void parseRegistrationResponse(const std::vector<uint8_t>& data, CurrentUser& currentUser);
	void parseClientsListResponse(const std::vector<uint8_t>& data, UserInfoList& userInfoList);
	void parsePublicKeyResponse(const std::vector<uint8_t>& data, UserInfo& userInfo, EncryptionManager& encryptionManager);
	std::vector<Message> parseAwaitingMessagesResponse(const std::vector<uint8_t>& data, UserInfoList& userInfoList, EncryptionManager& encryptionManager);
	void parseSymmetricKeyRequestResponse(const std::vector<uint8_t>& data, const UserInfo& userInfo);
	void parseMessage(Message& message, EncryptionManager& encryptionManager);
	void parseTextMessagetResponse(const std::vector<uint8_t>& dat, const UserInfo& userInfo);
};