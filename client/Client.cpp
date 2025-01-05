#include "Client.h"


Client::Client()
	: userInterface(),
	userInfoList(),
	currentUser(),
	fileManager(),
	networkManager(fileManager),
	requestBuilder(),
	responseParser(),
	encryptionManager(nullptr) {
	checkRegistration();
}

void Client::checkRegistration() {
	auto infile = fileManager.openFileIfExists("my.info");
	if (!infile) {
		currentUser = CurrentUser();
		encryptionManager = std::make_unique<EncryptionManager>();
		return;
	}
	std::string name, clientID, privateKey;
	try {
		std::getline(*infile, name);
		std::getline(*infile, clientID);
		std::getline(*infile, privateKey, '\0');

		infile->close();
		currentUser = CurrentUser(name, clientID);
		encryptionManager = std::make_unique<EncryptionManager>(Base64Wrapper::decode(privateKey));
	}
	catch (std::exception e) {
		std::cerr << "incomplete data in my.info file, either fix or delete it to register again." << std::endl;
	}
}


void Client::run() {
	while (true) {
		userInterface.displayMenu();
		int selection = userInterface.getUserSelection();
		handleUserSelection(selection);
	}
}

void Client::handleUserSelection(int selection) {

	if (selection == 1) {
		registerClient();
		return;
	}
	// if the user did not chose to register, inform them that it is required for any other action.
	else if (!currentUser.isRegistered()) { 
		userInterface.printText("Please register first.");
		return;
	}
	switch (selection) {
	case 2:
		requestClientsList();
		break;
	case 3:
		requestPublicKey();
		break;
	case 4:
		requestWaitingMessages();
		break;
	case 5:
		sendTextMessage();
		break;
	case 6:
		sendSymmetricKeyRequest();
		break;
	case 7:
		sendOwnSymmetricKey();
		break;
	case 0:
		exitClient();
		break;
	default:
		userInterface.printText("Invalid selection. Please try again.");
	}
}


void Client::registerClient() {
	try {
		if (currentUser.isRegistered()) {
			userInterface.printText("Already registered.");
			return;
		}
		if (!currentUser.setName(userInterface.getInput("Enter username: "))) {
			userInterface.printText("Name must not surpass 254 letters.");
			return;
		}
		networkManager.connect();
		networkManager.sendData(requestBuilder.buildRegistrationRequest(currentUser, encryptionManager->getPublicKey()));
		std::vector<uint8_t> response;
		networkManager.receiveData(response);
		networkManager.disconnect();
		responseParser.parseRegistrationResponse(response, currentUser);
		auto outfile = fileManager.createFile("my.info");
		if (!outfile) {
			std::cerr << "Error: Cannot create my.info file.\n";
			return;
		}
		*outfile << currentUser.getName() << std::endl;
		*outfile << currentUser.getClientID() << std::endl;
		*outfile << Base64Wrapper::encode(encryptionManager->getPrivateKey()) << std::endl;
		outfile->close();
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		networkManager.disconnect();
	}
}

void Client::requestClientsList() {

	try {
		std::vector<uint8_t> request = requestBuilder.buildClientsListRequest(currentUser);
		networkManager.connect();
		networkManager.sendData(request);

		std::vector<uint8_t> response;
		networkManager.receiveData(response);

		responseParser.parseClientsListResponse(response, userInfoList);
		userInterface.printText("Registered clients:");
		userInfoList.printUsers();
		networkManager.disconnect();
	}
	catch (std::exception e) {
		std::cerr << e.what() << std::endl;
		networkManager.disconnect();
	}
}

void Client::requestPublicKey() {
	try {
	std::string targetName = userInterface.getInput("Enter username to request public key: ");
	UserInfo requestedUser = userInfoList.getUserByName(targetName);
	std::vector<uint8_t> request = requestBuilder.buildPublicKeyRequest(currentUser, requestedUser);
	networkManager.connect();
	networkManager.sendData(request);

	std::vector<uint8_t> response;
	networkManager.receiveData(response);

	responseParser.parsePublicKeyResponse(response, requestedUser, *encryptionManager);
	networkManager.disconnect();
	}
	catch (std::exception e) {
		std::cerr << e.what() << std::endl;
		networkManager.disconnect();
	}
}

void Client::requestWaitingMessages() {
	try {
		networkManager.connect();
		std::vector<uint8_t> request = requestBuilder.buildWaitingMessagesRequest(currentUser);
		networkManager.sendData(request);
		std::vector<uint8_t> response;
		networkManager.receiveData(response);
		std::vector<Message> messages = responseParser.parseAwaitingMessagesResponse(response, userInfoList, *encryptionManager);
		userInterface.printMessages(messages);
		networkManager.disconnect();
	}
	catch (std::exception e) {
		networkManager.disconnect();
	}
}

void Client::sendSymmetricKeyRequest() {
	try {
		networkManager.connect();
		std::string targetName = userInterface.getInput("Enter username to request symmetric key: ");
		UserInfo requestedUser = userInfoList.getUserByName(targetName);
		std::vector<uint8_t> request = requestBuilder.buildSymmetricKeyRequest(currentUser, requestedUser, *encryptionManager);
		networkManager.sendData(request);
		std::vector<uint8_t> response;
		networkManager.receiveData(response);
		responseParser.parseSymmetricKeyRequestResponse(response, requestedUser);
		networkManager.disconnect();
	}
	catch (std::exception e) {
		networkManager.disconnect();
	}
}

void Client::sendTextMessage() {
	try {
		networkManager.connect();
		std::string targetName = userInterface.getInput("Enter username to send a messgae to: ");
		std::string textMessage = userInterface.getInput("Enter the message you'de like to send to this user: ");
		UserInfo requestedUser = userInfoList.getUserByName(targetName);
		
		std::vector<uint8_t> request = requestBuilder.buildTextMessageRequest(currentUser, requestedUser, textMessage, *encryptionManager);
		networkManager.sendData(request);
		std::vector<uint8_t> response;
		networkManager.receiveData(response);
		responseParser.parseTextMessagetResponse(response, requestedUser);
		networkManager.disconnect();
	}
	catch (std::exception e) {
		std::cerr << e.what() << std::endl;
		networkManager.disconnect();
	}
}

void Client::sendOwnSymmetricKey() {
	try {
		networkManager.connect();
		std::string targetName = userInterface.getInput("Enter username to send a symmetric key to: ");
		UserInfo requestedUser = userInfoList.getUserByName(targetName);

		std::vector<uint8_t> request = requestBuilder.buildSendSymmetricKey(currentUser, requestedUser, *encryptionManager);
		networkManager.sendData(request);
		std::vector<uint8_t> response;
		networkManager.receiveData(response);
		responseParser.parseSymmetricKeyRequestResponse(response, requestedUser);
		networkManager.disconnect();
	}
	catch (std::exception e) {
		std::cerr << e.what() << std::endl;
		networkManager.disconnect();
	}
}

void Client::exitClient() {
	exit(0);
}