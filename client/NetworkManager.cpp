#include "NetworkManager.h"



NetworkManager::NetworkManager() : socket(ioService) {
    if (!readServerInfo("server.info")) {
        IP = "127.0.0.1";
        port = 1357;
    }
}

bool NetworkManager::connect() {
    try {
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(IP), port);
        socket.connect(endpoint);
        return true;
    }
    catch (std::exception& e) {
        std::cerr << "Network connection error: " << e.what() << std::endl;
        return false;
    }
}

void NetworkManager::disconnect() {
    socket.close();
}

bool NetworkManager::readServerInfo(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        return false;
    }
    std::string line;
    if (std::getline(infile, line)) {
        size_t colon = line.find(':');
        if (colon != std::string::npos) { /* not the end of the file (means a colon exist) */
            IP = line.substr(0, colon);
            port = std::stoi(line.substr(colon + 1));
            return true;
        }
    }
    return false;
}


bool NetworkManager::sendData(const std::vector<uint8_t>& data) {
    try {
        boost::asio::write(socket, boost::asio::buffer(data));
        return true;
    }
    catch (std::exception& e) {
        std::cerr << "Error while sending data: " << e.what() << std::endl;
        return false;
    }
}

bool NetworkManager::receiveData(std::vector<uint8_t>& data) {
    try {
        // Read response header (version, code, payload size)
        uint8_t header[7];
        size_t bytesRead = boost::asio::read(socket, boost::asio::buffer(header, 7));
        if (bytesRead != 7) return false;
        // Extract the payload size 
        uint32_t payloadSize = header[3] | (header[4] << 8) | (header[5] << 16) | (header[6] << 24);
        data.insert(data.end(), header, header + 7);

        // Read payload according to the payload size
        if (payloadSize > 0) {
            std::vector<uint8_t> payload(payloadSize);
            bytesRead = boost::asio::read(socket, boost::asio::buffer(payload));
            if (bytesRead != payloadSize) return false;
            data.insert(data.end(), payload.begin(), payload.end());
        }
        return true;
    }
    catch (std::exception& e) {
        std::cerr << "Receive error: " << e.what() << std::endl;
        return false;
    }
}