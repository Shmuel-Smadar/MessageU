/*
 * File Name: Message.h
 *
 * This file represents a message in the app, store sender information, type of the message
 * and the decrypted content.
 */


#pragma once

#include <string>
#include <cstdint>
#include <iostream>
#include "ProtocolConstants.h"
#include "UserInfo.h"
#include "ClientException.h"

class Message {
private:
    const UserInfo& userInfo;
    uint8_t messageType;
    std::string content;

public:
    Message(UserInfo& userInfo, const uint8_t& messageType,const std::string& content);

    std::string getSenderClientId() const;
    std::string getSenderUserName() const;
    uint8_t getMessageType() const;
    std::string getContent() const;
    UserInfo getUser() const;
    void setContent(const std::string& content);
    std::string toString() const;
};