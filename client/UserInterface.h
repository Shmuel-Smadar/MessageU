#pragma once

#include <string>
#include <iostream>
#include "Utils.h"

class UserInterface {
public:
    void displayMenu() const;
    int getUserSelection() const;
    std::string getInput(const std::string& prompt) const;
    void printMessage(const std::string& message) const;
};