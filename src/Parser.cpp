#include "Parser.h"

#include <iostream>

Parser::Parser(std::vector<char> characters) : scanner(characters) {
}

void Parser::parse() {
	while (true) {
		Token t = scanner.scanToken();
		if (t.type == "END") {
			return;
		} else if (t.type == "ERREND") {
			std::cout << t.text << std::endl;
		}

		if (t.text == "") {
			std::cout << t.type << std::endl;
		} else {
			std::cout << t.type << "(" << t.text << ")" << std::endl;
		}
	}
}