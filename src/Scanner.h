#pragma once

#include <vector>
#include<iterator>
#include "Token.h"

class Scanner {
	public:
		Scanner(std::vector<char> characters);
		Token scanToken();

	private:
		std::vector<char> chars;
		std::vector<char>::iterator it;
};