#pragma once

#include <string>
#include <iostream>
#include "Utils.h"
#include "Message.h"

class UserInterface {
public:
    void displayMenu() const;
    int getUserSelection() const;
    std::string getInput(const std::string& prompt) const;
    void printText(const std::string& message) const;
    void printMessages(std::vector<Message> messages) const;
};