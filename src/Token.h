#pragma once

#include <string>

struct Token {
	std::string type = "";
	std::string text = "";
	// add line and col if there's time
};