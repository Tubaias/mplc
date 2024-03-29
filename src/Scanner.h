#pragma once

#include <vector>
#include <iterator>
#include <set>
#include "struct/Token.h"

class Scanner {
	public:
		Scanner(std::vector<char> characters);
		Token scanToken();
		void reset();

	private:
		std::set<std::string> keywords;
		std::vector<char> chars;
		std::vector<char>::iterator it;
		void stepBack();
		int line;
		char nextChar();
};