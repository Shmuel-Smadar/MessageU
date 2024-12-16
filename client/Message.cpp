#include "Message.h"


Message::Message(const std::string& senderClientId, const std::string& messageType, const std::string& content) {
	this->senderClientId = senderClientId;
	this->messageType = messageType;
	this->content = content;
}

std::string Message::getSenderClientId() const {
	return senderClientId;
}

std::string Message::getMessageType() const {
	return messageType;
}

std::string Message::getContent() const {
	return content;
}
void Message::printMessage() const {

}


