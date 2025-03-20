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
	void connect();
	void disconnect();
	bool readServerInfo(const std::string& filename);
	void sendAndReceive(const std::vector<uint8_t>& request, std::vector<uint8_t>& response);
private:
	void sendData(const std::vector<uint8_t>& data);
	void receiveData(std::vector<uint8_t>& data);
	boost::asio::io_context ioContext;
	boost::asio::ip::tcp::socket socket;
	std::string IP;
	uint16_t port;

	const std::string DEFAULT_IP = "127.0.0.1";
	const uint16_t DEFAULT_PORT = 1357;
	const std::string SERVER_INFO_FILENAME = "server.info";
};
