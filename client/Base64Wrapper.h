/*
 * File Name: Base64Wrapper .h
 *
 * This file is responsible for managing Base64 encryption and decryption in the app.
 * uses CryptoPP library and is provided by the university.
 */

#pragma once

#include <string>
#include <base64.h>


class Base64Wrapper
{
public:
	static std::string encode(const std::string& str);
	static std::string decode(const std::string& str);
};
