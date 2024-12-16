#pragma once

#include <string>
#include <cstdint>

class Message {
private:
    std::string senderClientId;
    uint8_t messageType;
    std::string content;
    //TODO: add checks that the content size isnt bigger than 32 bytes.

public:
    Message(const std::string& senderClientId, const uint8_t& messageType,const std::string& content);

    std::string getSenderClientId() const;
    uint8_t getMessageType() const;
    std::string getContent() const;
    void printMessage() const;


};