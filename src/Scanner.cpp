#include "Scanner.h"

#include <iostream>

Scanner::Scanner(std::vector<char> characters) {
	chars = characters;
}

void Scanner::scan() {
	for (char c : chars) {
		std::cout << c;
	}

	std::cout << std::endl;
}