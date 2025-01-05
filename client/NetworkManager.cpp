#include "NetworkManager.h"



NetworkManager::NetworkManager(FileManager fileManager) : socket(ioService) {

	try {
		auto infile = fileManager.openFileIfExists("server.info");
		if (!infile)
			throw std::runtime_error("File server.info doesn't exist.");
		std::string line;
		if (std::getline(*infile, line)) {
			size_t pos = line.find(':');
			if (pos != std::string::npos) {
				IP = line.substr(0, pos);
				port = std::stoi(line.substr(pos + 1));
			}
			else {
				throw std::runtime_error("Invalid format of server.info file.");
			}
		}
		else {
			throw std::runtime_error("Empty file server.info file.");
		}
	}
	catch (std::exception& e) {
		IP = "127.0.0.1";
		port = 1357;
		std::cerr << e.what() << std::endl;
		std::cerr << "using default address. " << IP << ":" << port << std::endl;
	}
}

void NetworkManager::connect() {
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(IP), port);
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
		if (colon != std::string::npos) { // not the end of the file (means a colon exist)
			IP = line.substr(0, colon);
			port = std::stoi(line.substr(colon + 1));
			return true;
		}
	}
	return false;
}


void NetworkManager::sendData(const std::vector<uint8_t>& data) {
	boost::asio::write(socket, boost::asio::buffer(data));
}
// TODO: fix magic numbers
void NetworkManager::receiveData(std::vector<uint8_t>& data) {
	// read response header (version, code, payload size)
	uint8_t header[7];
	size_t bytesRead = boost::asio::read(socket, boost::asio::buffer(header, 7));
	if (bytesRead != 7)
		throw std::runtime_error("Message recieved from the server does not have proper headers.");
	// extract the payload size 
	uint32_t payloadSize = header[3] | (header[4] << 8) | (header[5] << 16) | (header[6] << 24);
	data.insert(data.end(), header, header + 7);

	// read payload according to the payload size
	if (payloadSize > 0) {
		std::vector<uint8_t> payload(payloadSize);
		bytesRead = boost::asio::read(socket, boost::asio::buffer(payload));
		if (bytesRead != payloadSize)
			throw std::runtime_error("Message recieved from the server has shorter payload than what was specified.");
		data.insert(data.end(), payload.begin(), payload.end());
	}
}