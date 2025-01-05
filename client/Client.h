#pragma once
#include <stdexcept>
#include <memory>
#include "CurrentUser.h"
#include "UserInterface.h"
#include "UserInfoList.h"
#include "Utils.h"
#include "FileManager.h"
#include "EncryptionManager.h"
#include "NetworkManager.h"
#include "RequestBuilder.h"
#include "ResponseParser.h"
#include "Base64Wrapper.h"

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
    void requestWaitingMessages();
    void sendSymmetricKeyRequest();
    void sendTextMessage();
    void sendOwnSymmetricKey();
    void exitClient(); 

    
   
    RequestBuilder requestBuilder;
    ResponseParser responseParser;
    UserInterface userInterface;
    NetworkManager networkManager;
    UserInfoList userInfoList;
    CurrentUser currentUser;
    FileManager fileManager;
    std::unique_ptr<EncryptionManager> encryptionManager;
};
