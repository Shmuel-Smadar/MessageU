/*
* File Name: ErrorCodes.h
* This file contains the errors that will be displayed to the user.
*/


#pragma once
#include <string>
#include <array>

enum class ClientErrorCode : int {
    MISSING_SERVER_INFO_FILE = 0,
    INVALID_SERVER_INFO_FORMAT,
    EMPTY_SERVER_INFO,
    CONNECTION_ERROR,
    SEND_RECEIVE_ERROR,
    INVALID_SERVER_RESPONSE,
    DEFAULT_IP_USED,
    NAME_TOO_LONG,
    PUBLIC_KEY_NOT_FOUND,
    SYMMETRIC_KEY_NOT_FOUND,
    USER_NOT_FOUND,
    GENERAL_ERROR,
    FILE_PATH_ERROR,
    DIR_PARSE_ERROR,
    OS_NOT_SUPPORTED,
    RESPONSE_CODE_MISMATCH,
    RESPONSE_HEADER_INVALID,
    SERVER_RETURNED_ERROR,
    NUM_ERROR_CODES
};

namespace ErrorMessages {
    inline const std::array<std::string, static_cast<size_t>(ClientErrorCode::NUM_ERROR_CODES)> ERROR_STRINGS{
        "The file server.info is missing.",
        "The file server.info format is invalid.",
        "The file server.info is empty.",
        "Failed to connect to the server.",
        "An error occurred while sending or receiving data.",
        "Received an invalid response from the server.",
        "Server info unavailable; using default IP and port.",
        "Name must not surpass 254 letters.",
        "Public key of the requested user was not found.",
        "Symmetric key of the requested user was not found.",
        "User not found.",
        "An unexpected error occurred.",
        "Unable to retrieve executable path.",
        "Unable to parse executable directory.",
        "Currently only windows is supported.",
        "Response code mismatch.",
        "Server response does not meet header sizes defined in the protocol.",
        "Server responded with an error."
    };

    inline std::string getErrorString(ClientErrorCode code) {
        size_t index = static_cast<size_t>(code);
        if (index < ERROR_STRINGS.size()) {
            return ERROR_STRINGS[index];
        }
        return "Unknown error code.";
    }
}
