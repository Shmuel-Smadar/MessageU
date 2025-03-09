#include "Message.h"

Message::Message(UserInfo& userInfo, const uint8_t& messageType, const std::string& content)
	: userInfo(userInfo),
	messageType(messageType),
	content(content) {
}

std::string Message::getSenderClientId() const {
	return this->userInfo.getClientID();
}

std::string Message::getSenderUserName() const {
	return this->userInfo.getName();
}
uint8_t Message::getMessageType() const {
	return messageType;
}

std::string Message::getContent() const {
	return content;
}
UserInfo Message::getUser() const {
	return this->userInfo;
}
void Message::setContent(const std::string& content) {
	this->content = content;
}

std::string Message::toString() const {
	std::string messageString;
	messageString += "From: <" + this->getSenderUserName() + ">\n";
	messageString += "Content:\n";
	if (this->getMessageType() == MessageType::SymmetricKeyRequest) {
		messageString += "Request for symmetric key";
	}
	else if (this->getMessageType() == MessageType::SymmetricKeySent) {
		messageString += "Symmetric key received";
	}
	else if (this->getMessageType() == MessageType::TextMessageSent) {
		messageString += this->getContent();
	}
	messageString += "\n-----<EOM>-----";
	return messageString;
}