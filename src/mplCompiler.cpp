﻿#include "mplCompiler.h"

int main(int argc, char* argv[]) {
	std::cout << "Starting.\n" << std::endl;

	if (argc < 2) {
		std::cout << "Missing filename argument." << std::endl;
		return 1;
	}

	std::string filename(argv[1]);

	// read the file into a char vector
	FileReader reader(filename);
	std::vector<char> chars = reader.read();

	if (chars.empty()) {
		return 1;
	}

	// scanning
	Scanner scanner(chars);
	scanner.scan();

	return 0;
}