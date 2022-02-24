#pragma once

#include <vector>

class Scanner {
	public:
		Scanner(std::vector<char> characters);
		void scan();

	private:
		std::vector<char> chars;
};