#pragma once
#include <stdexcept>
#include "CurrentUser.h"
#include "UserInterface.h"
#include "UserInfoList.h"
#include "Utils.h"
#include "EncryptionManager.h"
#include "NetworkManager.h"
#include "RequestBuilder.h"
#include "ResponseParser.h"

class Client {
public:
    Client();
    void run();

private:

    void handleUserSelection(int selection);
    void checkRegistration();
    void registerClient();
    void requestClientsList();
    void requestPublicKey();

    /*TODO:
    void requestWaitingMessages();
    void sendTextMessage();
    void sendSymmetricKeyRequest();
    void sendOwnSymmetricKey();
    void exitClient();*/

    RequestBuilder requestBuilder;
    ResponseParser responseParser;
    UserInterface userInterface;
    NetworkManager networkManager;
    UserInfoList userInfoList;
    CurrentUser currentUser;
    std::unique_ptr<EncryptionManager> encryptionManager;
};
