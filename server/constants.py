# filename: constants.py
# this file contains protocol related constants that are used throughout the server

class ClientCodes:
    REGISTRATION = 600
    CLIENT_LIST = 601
    PUBLIC_KEY = 602
    MESSAGE_SENT = 603
    AWAITING_MESSAGES = 604

class ServerCodes:
    VERSION = 2
    REGISTRATION_SUCCESSFUL = 2100
    RETURNED_CLIENT_LIST = 2101
    RETURNED_PUBLIC_KEY = 2102
    MESSAGE_SENT = 2103
    RETURNED_AWAITING_MESSAGES = 2104
    ERROR = 9000

class ProtocolByteSizes:
    VERSION = 1
    CODE = 2
    PAYLOAD_SIZE = 4
    NAME = 255
    CLIENT_ID = 16
    PUBLIC_KEY = 160
    HEADER = 23 #CLIENT_ID + VERSION + CODE + PAYLOAD_SIZE = 16 + 1 + 4 + 2 = 23
    MESSAGE_ID = 4
    MESSAGE_TYPE = 1
    MESSAGE_LENGTH = 4
    MESSAGE_TYPE = 1
    MESSAGE_HEADER = 21 #CLIENT_ID + MESSAGE_TYPE + MESSAGE_LENGTH = 16 + 1 + 4 = 21