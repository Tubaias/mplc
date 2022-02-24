#include "FileReader.h"

#include <fstream>
#include<iostream>

FileReader::FileReader(std::string filename) {
	fName = filename;
}

std::vector<char> FileReader::read() {
	std::vector<char> chars;

	// open file stream
	std::ifstream file;
	file.open(fName, std::ios::in);

	// check that the file is valid
	if (!file.good()) {
		std::cout << "Could not find or access file at " << fName << " \n";
		return chars;
	}

	// save characters to vector
	char ch;
	while (file >> std::noskipws >> ch) {
		chars.push_back(ch);
	}

	file.close();
	return chars;
}