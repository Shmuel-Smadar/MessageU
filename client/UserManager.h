#pragma once

#include "LocalUser.h"
#include "UserInfoList.h"
#include "EncryptionManager.h"

class UserManager {

public:
    UserManager();
    void checkRegistration();
    std::unique_ptr<EncryptionManager> encryptionManager;
    UserInfoList userInfoList;

private:
    LocalUser localUser;
    
};