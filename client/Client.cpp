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
	//check if the file "me.info" exists. 
	auto infile = fileManager.openFileIfExists(INFO_FILENAME);
	if (!infile) {
		currentUser = CurrentUser();
		encryptionManager = std::make_unique<EncryptionManager>();
		return;
	}
	// if me.info exists, we try to use its data to initialize currentUser and encryptionManager.
	std::string name, clientID, privateKey;
	try {
		std::getline(*infile, name);
		std::getline(*infile, clientID);
		std::getline(*infile, privateKey, '\0');

		infile->close();
		currentUser = CurrentUser(name, clientID);
		encryptionManager = std::make_unique<EncryptionManager>(Base64Wrapper::decode(privateKey));
	}
	catch (std::exception&) {
		userInterface.printError(ErrorMessages::getErrorString(ClientErrorCode::INCOMPLETE_MY_INFO));
	}
}

void Client::run() {
	while (true) {
		userInterface.displayMenu();
		int selection = userInterface.getUserSelection();
		handleUserSelection(selection);
	}
}

void Client::handleException(const std::exception& e)
{
	const ClientException* clientEx = dynamic_cast<const ClientException*>(&e);
	if (clientEx) {
		userInterface.printError(e.what());
	}
	else {
		userInterface.printError(ErrorMessages::getErrorString(ClientErrorCode::GENERAL_ERROR));
	}
}

void Client::handleUserSelection(int selection) {
	if (selection == MenuOptions::Register) {
		registerClient();
		return;
	}
	else if (!currentUser.isRegistered()) {
		userInterface.printText(ErrorMessages::getErrorString(ClientErrorCode::PLEASE_REGISTER_FIRST));
		return;
	}

	switch (selection) {
	case MenuOptions::RequestClientsList:
		requestClientsList();
		break;
	case MenuOptions::RequestPublicKey:
		requestPublicKey();
		break;
	case MenuOptions::RequestWaitingMessages:
		requestWaitingMessages();
		break;
	case MenuOptions::SendTextMessage:
		sendTextMessage();
		break;
	case MenuOptions::RequestSymmetricKey:
		sendSymmetricKeyRequest();
		break;
	case MenuOptions::SendSymmetricKey:
		sendOwnSymmetricKey();
		break;
	case MenuOptions::Exit:
		exitClient();
		break;
	default:
		userInterface.printError(ErrorMessages::getErrorString(ClientErrorCode::INVALID_SELECTION));
	}
}


void Client::registerClient() {
	try {
		if (currentUser.isRegistered()) {
			userInterface.printText(ErrorMessages::getErrorString(ClientErrorCode::ALREADY_REGISTERED));
			return;
		}
		currentUser.setName(userInterface.getInput("Enter username: "));
		std::vector<uint8_t> request = requestBuilder.buildRegistrationRequest(currentUser, encryptionManager->getPublicKey());
		std::vector<uint8_t> response;
		networkManager.sendAndReceive(request, response);
		responseParser.parseRegistrationResponse(response, currentUser);
		auto outfile = fileManager.createFile(INFO_FILENAME);
		if (!outfile) {
			userInterface.printError(ErrorMessages::getErrorString(ClientErrorCode::CANNOT_CREATE_MY_INFO));
			return;
		}
		// write current user info into me
		// .info file for future runs of the program.
		*outfile << currentUser.getName() << std::endl;
		*outfile << currentUser.getClientID() << std::endl;
		*outfile << Base64Wrapper::encode(encryptionManager->getPrivateKey()) << std::endl;
		outfile->close();
	}
	catch (std::exception& e) {
		handleException(e);
	}
}

void Client::requestClientsList() {
	try {
		std::vector<uint8_t> request = requestBuilder.buildClientsListRequest(currentUser);
		std::vector<uint8_t> response;
		networkManager.sendAndReceive(request, response);
		responseParser.parseClientsListResponse(response, userInfoList);
		userInfoList.printUsers(userInterface);
	}
	catch (std::exception& e) {
		handleException(e);
	}
}

void Client::requestPublicKey() {
	try {
		std::string targetName = userInterface.getInput("Enter username to request public key: ");
		UserInfo requestedUser = userInfoList.getUserByName(targetName);
		std::vector<uint8_t> request = requestBuilder.buildPublicKeyRequest(currentUser, requestedUser);
		std::vector<uint8_t> response;
		networkManager.sendAndReceive(request, response);
		responseParser.parsePublicKeyResponse(response, requestedUser, *encryptionManager);
	}
	catch (std::exception& e) {
		handleException(e);
	}
}

void Client::requestWaitingMessages() {
	try {
		std::vector<uint8_t> request = requestBuilder.buildWaitingMessagesRequest(currentUser);
		std::vector<uint8_t> response;
		networkManager.sendAndReceive(request, response);
		std::vector<Message> messages = responseParser.parseAwaitingMessagesResponse(response, userInfoList, *encryptionManager);
		userInterface.printMessages(messages);
	}
	catch (std::exception& e) {
		handleException(e);
	}
}

void Client::sendSymmetricKeyRequest() {
	try {
		std::string targetName = userInterface.getInput("Enter username to request symmetric key: ");
		UserInfo requestedUser = userInfoList.getUserByName(targetName);
		std::vector<uint8_t> request = requestBuilder.buildSymmetricKeyRequest(currentUser, requestedUser, *encryptionManager);
		std::vector<uint8_t> response;
		networkManager.sendAndReceive(request, response);
		responseParser.parseSymmetricKeyRequestResponse(response, requestedUser);
	}
	catch (std::exception& e) {
		handleException(e);
	}
}

void Client::sendTextMessage() {
	try {
		std::string targetName = userInterface.getInput("Enter username to send a message to: ");
		std::string textMessage = userInterface.getInput("Enter the message you would like to send to this user: ");
		UserInfo requestedUser = userInfoList.getUserByName(targetName);
		std::vector<uint8_t> request = requestBuilder.buildTextMessageRequest(currentUser, requestedUser, textMessage, *encryptionManager);
		std::vector<uint8_t> response;
		networkManager.sendAndReceive(request, response);
		responseParser.parseTextMessageResponse(response, requestedUser);
	}
	catch (std::exception& e) {
		handleException(e);
	}
}

void Client::sendOwnSymmetricKey() {
	try {
		std::string targetName = userInterface.getInput("Enter username to send a symmetric key to: ");
		UserInfo requestedUser = userInfoList.getUserByName(targetName);
		std::vector<uint8_t> request = requestBuilder.buildSendSymmetricKey(currentUser, requestedUser, *encryptionManager);
		std::vector<uint8_t> response;
		networkManager.sendAndReceive(request, response);
		responseParser.parseSymmetricKeyRequestResponse(response, requestedUser);
	}
	catch (std::exception& e) {
		handleException(e);
	}
}

void Client::exitClient() {
	exit(0);
}