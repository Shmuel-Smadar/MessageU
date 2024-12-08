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
