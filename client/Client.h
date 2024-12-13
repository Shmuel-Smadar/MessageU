#pragma once
#include <stdexcept>
#include "CurrentUser.h"
#include "UserInterface.h"
#include "UserInfoList.h"
#include "Utils.h"
#include "EncryptionManager.h"
#include "NetworkManager.h"
#include "ProtocolHandler.h"


class Client {
public:
    Client();
    void run();

private:

    void handleUserSelection(int selection);
    void checkRegistration();
    void registerClient();
    /*TODO:
    void requestClientsList();
    void requestPublicKey();
    void requestWaitingMessages();
    void sendTextMessage();
    void sendSymmetricKeyRequest();
    void sendOwnSymmetricKey();
    void exitClient();*/


    
    UserInterface userInterface;
    NetworkManager networkManager;
    UserInfoList userInfoList;
    CurrentUser currentUser;
    ProtocolHandler protocolHandler;
    std::unique_ptr<EncryptionManager> encryptionManager;
};
