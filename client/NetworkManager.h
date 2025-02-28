#pragma once

#include <boost/asio.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include "ProtocolConstants.h"
#include "FileManager.h"
#include "Utils.h"

class NetworkManager {
public:
    NetworkManager(FileManager fileManager);
    void connect();
    void disconnect();
    void sendData(const std::vector<uint8_t>& data);
    void receiveData(std::vector<uint8_t>& data);
private:
    std::string IP;
    uint16_t port;
    boost::asio::io_context ioContext;
    boost::asio::ip::tcp::socket socket;
    bool readServerInfo(const std::string& filename);
};
