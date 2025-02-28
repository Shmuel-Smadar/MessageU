#include "NetworkManager.h"

NetworkManager::NetworkManager(FileManager fileManager)
    : socket(ioContext)
{
    try {
        auto infile = fileManager.openFileIfExists("server.info");
        if (!infile)
            throw std::runtime_error("File server.info doesn't exist.");

        std::string line;
        if (std::getline(*infile, line)) {
            size_t pos = line.find(':');
            if (pos != std::string::npos) {
                IP = line.substr(0, pos);
                port = static_cast<uint16_t>(std::stoi(line.substr(pos + 1)));
            }
            else {
                throw std::runtime_error("Invalid format of server.info file.");
            }
        }
        else {
            throw std::runtime_error("Empty server.info file.");
        }
    }
    catch (const std::exception& e) {
        IP = "127.0.0.1";
        port = 1357;
        std::cerr << e.what() << std::endl;
        std::cerr << "Using default address: " << IP << ":" << port << std::endl;
    }
}

void NetworkManager::connect() {
    boost::asio::ip::tcp::endpoint endpoint(
        boost::asio::ip::make_address(IP), static_cast<unsigned short>(port)
    );
    socket.connect(endpoint);
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
        if (colon != std::string::npos) {
            IP = line.substr(0, colon);
            port = static_cast<uint16_t>(std::stoi(line.substr(colon + 1)));
            return true;
        }
    }
    return false;
}

void NetworkManager::sendData(const std::vector<uint8_t>& data) {
    boost::asio::write(socket, boost::asio::buffer(data));
}

void NetworkManager::receiveData(std::vector<uint8_t>& data) {
    // read response header (version, code, payload size)
    uint8_t header[ProtocolByteSizes::Header];
    size_t bytesRead = boost::asio::read(socket, boost::asio::buffer(header, ProtocolByteSizes::Header));
    if (bytesRead != ProtocolByteSizes::Header)
        throw std::runtime_error("Message received from the server does not have proper headers.");
    // extract the payload size 
    std::vector<uint8_t> sizeBytes(header + ProtocolByteSizes::Version + ProtocolByteSizes::Code,
        header + ProtocolByteSizes::Header);
    uint32_t payloadSize = Utils::parseUint32(sizeBytes);
    data.insert(data.end(), header, header + ProtocolByteSizes::Header);

    // read payload according to the payload size
    if (payloadSize > 0) {
        std::vector<uint8_t> payload(payloadSize);
        bytesRead = boost::asio::read(socket, boost::asio::buffer(payload));
        if (bytesRead != payloadSize)
            throw std::runtime_error("Message received from the server has a shorter payload than specified.");
        data.insert(data.end(), payload.begin(), payload.end());
    }
}