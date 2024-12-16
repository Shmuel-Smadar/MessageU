#pragma once

#include <string>

class Message {
private:
    std::string senderClientId;
    std::string messageType;
    std::string content;

public:
    Message(const std::string& senderClientId, const std::string& messageType,const std::string& content);

    std::string getSenderClientId() const;
    std::string getMessageType() const;
    std::string getContent() const;
    void printMessage() const;


};