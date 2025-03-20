#include "NetworkManager.h"

NetworkManager::NetworkManager(FileManager fileManager)
	: socket(ioContext)
{
	try {
		auto infile = fileManager.openFileIfExists(SERVER_INFO_FILENAME);
		if (!infile)
			throw ClientException(ClientErrorCode::MISSING_SERVER_INFO_FILE);
		std::string line;
		if (std::getline(*infile, line)) {
			size_t pos = line.find(':');
			if (pos != std::string::npos) {
				IP = line.substr(0, pos);
				port = static_cast<uint16_t>(std::stoi(line.substr(pos + 1)));
			}
			else {
				throw ClientException(ClientErrorCode::INVALID_SERVER_INFO_FORMAT);
			}
		}
		else {
			throw ClientException(ClientErrorCode::EMPTY_SERVER_INFO);
		}
	}
	catch (const std::exception& e) {
		IP = DEFAULT_IP;
		port = DEFAULT_PORT;
		std::cerr << e.what() << std::endl;
		std::cerr << "Using default address: " << IP << ":" << port << std::endl;
	}
}

void NetworkManager::connect() {
	try {
		boost::asio::ip::tcp::endpoint endpoint(
			boost::asio::ip::make_address(IP), static_cast<unsigned short>(port)
		);
		socket.connect(endpoint);
	}
	catch (...) {
		throw ClientException(ClientErrorCode::CONNECTION_ERROR);
	}
}

void NetworkManager::disconnect() {
	try {
		socket.close();
	}
	catch (...) {
		throw ClientException(ClientErrorCode::CONNECTION_ERROR);
	}
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

void NetworkManager::sendAndReceive(const std::vector<uint8_t>& request, std::vector<uint8_t>& response) {
	connect();
	try {
		sendData(request);
		receiveData(response);
		disconnect();
	}
	catch (...) {
		disconnect();
		throw ClientException(ClientErrorCode::SEND_RECEIVE_ERROR);
	}
}

void NetworkManager::sendData(const std::vector<uint8_t>& data) {
	boost::asio::write(socket, boost::asio::buffer(data));
}

void NetworkManager::receiveData(std::vector<uint8_t>& data) {
	uint8_t header[ProtocolByteSizes::Header];
	size_t bytesRead = boost::asio::read(socket, boost::asio::buffer(header, ProtocolByteSizes::Header));
	if (bytesRead != ProtocolByteSizes::Header)
		throw ClientException(ClientErrorCode::INVALID_SERVER_RESPONSE);
	std::vector<uint8_t> sizeBytes(header + ProtocolByteSizes::Version + ProtocolByteSizes::Code,
		header + ProtocolByteSizes::Header);
	uint32_t payloadSize = Utils::parseUint32(sizeBytes);
	data.insert(data.end(), header, header + ProtocolByteSizes::Header);
	if (payloadSize > 0) {
		std::vector<uint8_t> payload(payloadSize);
		bytesRead = boost::asio::read(socket, boost::asio::buffer(payload));
		if (bytesRead != payloadSize)
			throw ClientException(ClientErrorCode::INVALID_SERVER_RESPONSE);
		data.insert(data.end(), payload.begin(), payload.end());
	}
}