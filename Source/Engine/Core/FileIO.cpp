#include "FileIO.h"
#include <fstream>

namespace lola
{
	std::string getFilePath()
	{
		return std::filesystem::current_path().string();
	}

	bool setFilePath(const std::filesystem::path& path)
	{
		std::error_code ec;
		std::filesystem::current_path(path, ec);

		return ec.value() == 0;
	}

	bool fileExists(const std::filesystem::path& path)
	{
		return std::filesystem::exists(path);
	}

	std::string getFileName(const std::filesystem::path& path)
	{
		return path.filename().string();
	}

	bool getFileSize(const std::filesystem::path& path, size_t& size)
	{
		std::error_code ec;
		size = std::filesystem::file_size(path, ec);

		return ec.value() == 0;
	}

	bool readFile(const std::filesystem::path& path, std::string& buffer)
	{
		if (!fileExists(path)) return false;

		size_t size;
		if (!getFileSize(path, size)) return false;

		buffer.resize(size);
				
		std::ifstream stream(path);
		stream.read(buffer.data(), size);
		stream.close();

		return true;
	}
}
