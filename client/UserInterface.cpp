#include "UserInterface.h"


void UserInterface::displayMenu() const {
    std::cout << "\nMessageU client at your service.\n\n"
        << "1) Register\n"
        << "2) Request for clients list\n"
        << "3) Request for public key\n"
        << "4) Request for waiting messages\n"
        << "5) Send a text message\n"
        << "6) Send a request for symmetric key\n"
        << "7) Send your symmetric key\n"
        << "0) Exit client\n\n"
        << "? ";
}

int UserInterface::getUserSelection() const {

    std::string input;
    std::getline(std::cin, input);
    try {
        int selection = Utils::stringToInt(input);
        return selection;
    }
    catch (std::exception e) {
        return -1;
    }
}

std::string UserInterface::printText(const std::string& prompt) const {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

void UserInterface::printMessage(const std::string& message) const {
    std::cout << message << std::endl;
}

void UserInterface::printMessages(std::vector<Message> messages) const {
    for (Message message : messages) {
        message.printMessage();
    }
}