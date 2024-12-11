#include "Client.h"


Client::Client()
	: userInterface(),
	userInfoList(),
	currentUser(),
	networkManager(),
	protocolHandler(),
	encryptionManager(nullptr) {
	checkRegistration();
}

void Client::checkRegistration() {
	std::ifstream infile("my.info");
	if (!infile.is_open()) {
		currentUser = CurrentUser();
		encryptionManager = std::make_unique<EncryptionManager>();
		return;
	}

	std::string name, clientID, privateKey;
	if (!std::getline(infile, name) || !std::getline(infile, clientID) || !std::getline(infile, privateKey)) {
		std::cerr << "Error: Incomplete data in my.info file.\n";
		exit(0);
	}

	infile.close();
	currentUser = CurrentUser(name, clientID);
	encryptionManager = std::make_unique<EncryptionManager>(privateKey);
}


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
	if (currentUser.isRegistered()) {
		userInterface.printMessage("Already registered.");
		return;
	}
	currentUser.setName(userInterface.getInput("Enter username: "));
	
	std::string publicKey = encryptionManager->getPublicKey();
	networkManager.connect();
	networkManager.sendData(protocolHandler.buildRegistrationRequest(currentUser, encryptionManager->getPublicKey()));
}
