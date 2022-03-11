#pragma once

#include <string>

struct Token {
	std::string type = ""; // This should be turned into an enum eventually
	std::string text = "";
	int line = 0;
	// add col if there's time
};