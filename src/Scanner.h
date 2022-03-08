#pragma once

#include <vector>
#include <iterator>
#include <set>
#include "Token.h"

class Scanner {
	public:
		Scanner(std::vector<char> characters);
		Token scanToken();

	private:
		std::set<std::string> keywords;
		std::vector<char> chars;
		std::vector<char>::iterator it;	
		char nextChar();
};