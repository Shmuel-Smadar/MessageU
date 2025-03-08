#pragma once
#include <stdexcept>
#include "ErrorCodes.h"

class ClientException : public std::runtime_error {
public:
    explicit ClientException(ClientErrorCode code)
        : std::runtime_error(ErrorMessages::getErrorString(code)) {}
};