#pragma once

#include <vector>
#include <string>

struct ASTnode {
	std::vector<ASTnode> children;
	std::string type = ""; // This should be turned into an enum eventually
	std::string text = "";
	int line = 0;
};