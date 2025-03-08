/*
* File Name: ClientException.h
 * This file represents custom exceptions in the client.
 * this way we can check if the exception is of this type, and if so,
 * we know the string it contains can be displayed to the user.
*/

#pragma once
#include <stdexcept>
#include "ErrorCodes.h"

class ClientException : public std::runtime_error {
public:
    explicit ClientException(ClientErrorCode code)
        : std::runtime_error(ErrorMessages::getErrorString(code)) {}
};