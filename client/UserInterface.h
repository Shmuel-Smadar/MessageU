/*
 * File Name: UserInterface.h
 *
 * This file is responsible for handling user interactions, like displaying the menu,
 * receiving user input, printing messages and errors.
 */


#pragma once

#include <string>
#include <iostream>
#include "Utils.h"
#include "Message.h"
#include "ProtocolConstants.h"

class UserInterface {
public:
	void displayMenu() const;
	int getUserSelection() const;
	std::string getInput(const std::string& prompt) const;
	void printText(const std::string& message) const;
	void printError(const std::string& message) const;
	void printMessages(std::vector<Message> messages) const;
};