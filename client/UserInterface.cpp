#include "UserInterface.h"


void UserInterface::displayMenu() const {
	std::cout << "\nMessageU client at your service.\n\n"
		<< int(MenuOptions::Register) << ") Register\n"
		<< int(MenuOptions::RequestClientsList) << ") Request for clients list\n"
		<< int(MenuOptions::RequestPublicKey) << ") Request for public key\n"
		<< int(MenuOptions::RequestWaitingMessages) << ") Request for waiting messages\n"
		<< int(MenuOptions::SendTextMessage) << ") Send a text message\n"
		<< int(MenuOptions::RequestSymmetricKey) << ") Send a request for symmetric key\n"
		<< int(MenuOptions::SendSymmetricKey) << ") Send your symmetric key\n"
		<< int(MenuOptions::Exit) << ") Exit client\n\n"
		<< "? ";
}

int UserInterface::getUserSelection() const {

	std::string input;
	std::getline(std::cin, input);
	try {
		int selection = Utils::stringToInt(input);
		return selection;
	}
	catch (...) {
		return -1;
	}
}

std::string UserInterface::getInput(const std::string& prompt) const {
	std::cout << prompt;
	std::string input;
	std::getline(std::cin, input);
	return input;
}

void UserInterface::printText(const std::string& message) const {
	std::cout << message << std::endl;
}

void UserInterface::printError(const std::string& message) const {
	std::cerr << message << std::endl;
}

void UserInterface::printMessages(std::vector<Message> messages) const {
	for (Message message : messages) {
		printText(message.toString());
	}
}