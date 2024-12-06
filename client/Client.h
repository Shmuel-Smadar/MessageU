#pragma once




#include "LocalUser.h"
#include "UserInterface.h"
#include "UserInfoList.h"
#include "Utils.h"

class Client {
public:
    Client();
    void run();

private:

    void handleUserSelection(int selection);
    /*TODO:
    void registerClient();
    void requestClientsList();
    void requestPublicKey();
    void requestWaitingMessages();
    void sendTextMessage();
    void sendSymmetricKeyRequest();
    void sendOwnSymmetricKey();
    void exitClient();*/


    UserInfoList userInfoList;
    LocalUser localUser;
    UserInterface userInterface;
};
