#pragma once

#include "Scanner.h"

class Parser {
	public:
		Parser(std::vector<char> characters);
		void parse();

	private:
		Scanner scanner;
		Token nextToken();
};