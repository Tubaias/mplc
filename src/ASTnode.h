#pragma once

#include <vector>
#include "Token.h"

struct ASTnode {
	std::vector<ASTnode> children;
	std::string type = "";
	std::string text = "";
};