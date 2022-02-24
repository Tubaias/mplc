#pragma once

#include <string>
#include <vector>

class FileReader {
	public:
		FileReader(std::string filename);
		std::vector<char> read();

	private:
		std::string fName;
};