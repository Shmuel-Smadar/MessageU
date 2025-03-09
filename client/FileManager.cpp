#include "FileManager.h"

#ifdef _WIN32
#include <windows.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#endif

FileManager::FileManager() {
	determineExecutableDir();
}

void FileManager::determineExecutableDir() {
#ifdef _WIN32
	char path[MAX_PATH];
	DWORD length = GetModuleFileNameA(NULL, path, MAX_PATH);
	if (length == 0 || length == MAX_PATH) {
		throw ClientException(ClientErrorCode::FILE_PATH_ERROR);
	}
	// Remove the executable name to get the directory
	if (!PathRemoveFileSpecA(path)) {
		throw ClientException(ClientErrorCode::DIR_PARSE_ERROR);
	}
	executableDir = std::filesystem::path(path);
#else
	throw ClientException(ClientErrorCode::OS_NOT_SUPPORTED);
#endif
}

std::filesystem::path FileManager::concatenatePath(const std::string& filename) const {
	return executableDir / filename;
}

std::unique_ptr<std::ifstream> FileManager::openFileIfExists(const std::string& filename) const {
	std::filesystem::path filePath = concatenatePath(filename);
	auto infile = std::make_unique<std::ifstream>(filePath);
	if (!infile->is_open()) {
		return nullptr;
	}
	return infile;
}

std::unique_ptr<std::ofstream> FileManager::createFile(const std::string& filename) const {
	std::filesystem::path filePath = concatenatePath(filename);

	auto outfile = std::make_unique<std::ofstream>(filePath);
	if (!outfile->is_open()) {
		return nullptr;
	}
	return outfile;
}
