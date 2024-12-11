#pragma once
#include <boost/asio.hpp>
#include <fstream>
#include <iostream>

class NetworkManager {
public:
    NetworkManager();
    bool connect();
    void disconnect();
    bool sendData(const std::vector<uint8_t>& data);

private:
    std::string IP;
    int port; //TODO: look for better type for the port
    boost::asio::io_service ioService;
    boost::asio::ip::tcp::socket socket;
    bool readServerInfo(const std::string& filename);
    
};