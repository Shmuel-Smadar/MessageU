/*
 * File Name: FileManager.h
 *
 * This file is responsible for handling file operations, including opening and creating files
 * in the executable (exe) directory. currently supports windows only.
 */
#pragma once

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include "ClientException.h"
#include "ErrorCodes.h"
#include <stdexcept>

class FileManager {

private:
	std::filesystem::path executableDir;
	void determineExecutableDir();
	std::filesystem::path concatenatePath(const std::string& filename) const;

public:
	FileManager();
	~FileManager() = default;

	std::unique_ptr<std::ifstream> openFileIfExists(const std::string& filename) const;
	std::unique_ptr<std::ofstream> createFile(const std::string& filename) const;

};
