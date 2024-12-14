class ClientCodes:
    REGISTRATION = 600
    CLIENT_LIST = 601

class ServerCodes:
    VERSION = 1
    REGISTRATION_SUCCESSFUL = 2100
    RETURNED_CLIENT_LIST = 2101
    ERROR = 9000

class ProtocolLengths:
    NAME = 255
    PUBLIC_KEY = 160
    HEADER = 23