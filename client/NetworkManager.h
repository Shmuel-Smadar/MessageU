/*
 * File Name: NetworkManager.h
 *
 * This file is responsible for Handling network communication with the server using Boost.
 * manages connecting, disconnecting, sending, and receiving data,
 * uses 'server.info' file to get the server information, otherwise uses default values.
 */

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
#include "ClientException.h"

class NetworkManager {
public:
	NetworkManager(FileManager fileManager);
	void sendAndReceive(const std::vector<uint8_t>& request, std::vector<uint8_t>& response);
private:
	std::string IP;
	uint16_t port;
	boost::asio::io_context ioContext;
	boost::asio::ip::tcp::socket socket;
	bool readServerInfo(const std::string& filename);
	void connect();
	void disconnect();
	void sendData(const std::vector<uint8_t>& data);
	void receiveData(std::vector<uint8_t>& data);
};
