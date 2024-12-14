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
		requestClientsList();
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
	try {
		if (currentUser.isRegistered()) {
			userInterface.printMessage("Already registered.");
			return;
		}
		if (!currentUser.setName(userInterface.getInput("Enter username: "))) {
			userInterface.printMessage("Name must not surpass 254 letters.");
			return;
		}
		if (networkManager.connect() == false)
			return;
		networkManager.sendData(protocolHandler.buildRegistrationRequest(currentUser, encryptionManager->getPublicKey()));
		std::vector<uint8_t> response;
		networkManager.receiveData(response);
		networkManager.disconnect();
		protocolHandler.parseRegistrationResponse(response, currentUser);
	}
	catch (std::exception& e) {
		return;
	}
}

void Client::requestClientsList() {
	if (!currentUser.isRegistered()) {
		userInterface.printMessage("Please register first.");
		return;
	}
	std::vector<uint8_t> request = protocolHandler.buildClientsListRequest(currentUser);
	if (!networkManager.connect()) {
		return;
	}
	if (!networkManager.sendData(request)) {
		networkManager.disconnect();
		return;
	}
	std::vector<uint8_t> response;
	if (!networkManager.receiveData(response)) {
		userInterface.printMessage("Failed to receive response from server.");
		networkManager.disconnect();
		return;
	}
	if(!protocolHandler.parseClientsListResponse(response, userInfoList))
		return; //TODO: rethink using exceptions and when to print errors
	userInterface.printMessage("Registered clients:");
	userInfoList.printUsers();
	networkManager.disconnect();
}
