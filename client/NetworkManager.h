#pragma once
#include <boost/asio.hpp>
#include <fstream>
#include <iostream>
#include "FileManager.h"

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
    boost::asio::io_service ioService;
    boost::asio::ip::tcp::socket socket;
    bool readServerInfo(const std::string& filename);
    
};