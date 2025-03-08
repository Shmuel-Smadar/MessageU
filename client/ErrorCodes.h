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
    INVALID_HEX_STRING,
    INVALID_INTEGER,
    INCOMPLETE_MY_INFO,
    CANNOT_CREATE_MY_INFO,
    ALREADY_REGISTERED,
    PLEASE_REGISTER_FIRST,
    INVALID_SELECTION,
    MESSAGE_TOO_LONG,
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
        "Server responded with an error.",
        "Invalid hex string.",
        "Invalid integer.",
        "Incomplete data in my.info file, either fix or delete it to register again.",
        "Error: Cannot create my.info file.",
        "Already registered.",
        "Please register first.",
        "Invalid selection. Please try again.",
        "Message content size must not exceeds 32 bytes."
    };
    inline std::string getErrorString(ClientErrorCode code) {
        size_t index = static_cast<size_t>(code);
        if (index < ERROR_STRINGS.size()) {
            return ERROR_STRINGS[index];
        }
        return "Unknown error code.";
    }
}
