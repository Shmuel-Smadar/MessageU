#pragma once

#include <string>
#include <cstdint>
#include <iostream>
#include "ProtocolConstants.h"
#include "UserInfo.h"

class Message {
private:
    UserInfo* userInfo;
    uint8_t messageType;
    std::string content;
    //TODO: add checks that the content size isnt bigger than 32 bytes.

public:
    Message(UserInfo* userInfo, const uint8_t& messageType,const std::string& content);

    std::string getSenderClientId() const;
    std::string getSenderUserName() const;
    uint8_t getMessageType() const;
    std::string getContent() const;
    UserInfo* getUser() const;
    void setContent(const std::string& content);
    void printMessage() const;
    

};