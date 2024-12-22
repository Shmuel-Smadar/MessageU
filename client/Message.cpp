#include "Message.h"


Message::Message(UserInfo* userInfo, const uint8_t& messageType, const std::string& content) {
	this->userInfo = userInfo;
	this->messageType = messageType;
	this->content = content;
}

std::string Message::getSenderClientId() const {
	return this->userInfo->getClientID();
}

std::string Message::getSenderUserName() const {
	return this->userInfo->getName();
}
uint8_t Message::getMessageType() const {
	return messageType;
}

std::string Message::getContent() const {
	return content;
}
UserInfo* Message::getUser() const {
	return this->userInfo;
}
void Message::setContent(const std::string& content) {
	this->content = content;
}

void Message::printMessage() const {
	std::cout << "From: <" << this->getSenderUserName() << ">" << std::endl;
	std::cout << "Content:" << std::endl;
	if (this->getMessageType() == MessageType::SymmetricKeyRequest) {
		std::cout << "Request for symmetric key" << std::endl;
	}
	else if (this->getMessageType() == MessageType::SymmetricKeySent) {
		std::cout << "Symmetric key received" << std::endl;
	}
	else if (this->getMessageType() == MessageType::TextMessageSent) {
		std::cout << this->getContent() << std::endl;
	}
	std::cout << "-----<EOM>-----" << std::endl;

}


