#include "Client.h"


Client::Client()
	: userInterface(),
	userInfoList(),
	localUser(),
	encryptionManager(),
	networkManager() { }


void Client::run() {
	while (true) {
		userInterface.displayMenu();
		int selection = userInterface.getUserSelection();
		handleUserSelection(selection);
	}
}

void Client::handleUserSelection(int selection) {
	switch (selection) {
	case 1:
		registerClient();
		break;
	case 2:
		//requestClientsList();
		break;
	case 3:
		//requestPublicKey();
		break;
	case 4:
		//requestWaitingMessages();
		break;
	case 5:
		//sendTextMessage();
		break;
	case 6:
		//sendSymmetricKeyRequest();
		break;
	case 7:
		//sendOwnSymmetricKey();
		break;
	case 0:
		//exitClient();
		break;
	default:
		userInterface.printMessage("Invalid selection. Please try again.");
	}
}


void Client::registerClient() {
	if (localUser.isRegistered()) {
		userInterface.printMessage("Already registered.");
		return;
	}
	std::string name = userInterface.getInput("Enter username: ");
	localUser.setName(name);

	std::string publicKey = encryptionManager.getPublicKey();
	networkManager.connect();
}
