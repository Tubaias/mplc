#include "SemanticAnalyzer.h"

SemanticAnalyzer::SemanticAnalyzer() {
	hadError = false;
}

void SemanticAnalyzer::error(ASTnode node, std::string msg) noexcept(false) {
	hadError = true;
	msg = "(" + node.type + ") " + msg + " (Line " + std::to_string(node.line) + ")";
	throw SemanticException(msg);
}

void SemanticAnalyzer::handleException(SemanticException e) {
	std::cout << "SEMANTIC ERROR: " << e.what() << std::endl;
}

void SemanticAnalyzer::printSymboltable() {
	std::cout << "symbol table:" << std::endl;

	for (std::pair<std::string, VarItem> pair : symboltable) {
		std::cout << pair.first << ": (" << pair.second.type << ") :: " << pair.second.value << std::endl;
	}
}

// semantic analysis entry point
std::unordered_map<std::string, VarItem> SemanticAnalyzer::analyze(ASTnode ast) {
	for (ASTnode child : ast.children) {
		try {
			analyzeNode(child);
		} catch (SemanticException e) {
			handleException(e);
		}
	}

	if (hadError) {
		printSymboltable();
	}

	return symboltable;
}

void SemanticAnalyzer::analyzeNode(ASTnode node) {
	if (node.type == "var") {
		// check that variable doesn't exist already and create it
		std::string name = node.children.at(0).text;
		if (symboltable.find(name) != symboltable.end()) {
			error(node.children.at(0), "Variable already exists");
		}

		std::string type = node.children.at(1).text;
		VarItem v;
		v.type = type;

		if (type == "int") {
			v.value = "0";
		} else if (type == "string") {
			v.value = "";
		} else {
			v.value = "false";
		}

		// check that assigned value matches type
		if (node.children.size() == 3) {
			if (type == "int") {
				isNumberType(node.children.at(2));
			} else if (type == "string") {
				isStringType(node.children.at(2));
			} else {
				isBoolType(node.children.at(2));
			}
		}

		symboltable[name] = v;
	} else if (node.type == "assign") {
		identExists(node.children.at(0));

		std::string type = symboltable.at(node.children.at(0).text).type;
		if (type == "int") {
			isNumberType(node.children.at(1));
		} else if (type == "string") {
			isStringType(node.children.at(1));
		} else {
			isBoolType(node.children.at(1));
		}
	} else if (node.type == "loop") {
		identExists(node.children.at(0));
		isNumberType(node.children.at(1));
		isNumberType(node.children.at(2));

		for (int i = 3; i < node.children.size(); i++) {
			ASTnode child = node.children.at(i);
			if (child.type == "assign" && child.children.at(0).text == node.children.at(0).text) {
				error(child, "Attempting to assign value to loop control valuable inside loop");
			}
		}
	} else if (node.type == "read") {
		identExists(node.children.at(0));
	} else if (node.type == "print") {
		// check that operand is int or string
		ASTnode child = node.children.at(0);

		if (child.type != "text" && child.type != "number" && child.type != "add"
			&& child.type != "sub" && child.type != "mul" && child.type != "div") {
			if (child.type == "ident") {
				identExists(child);
				if (symboltable.at(child.text).type != "string" && symboltable.at(child.text).type != "int") {
					error(child, "Incorrect operand type, expected string or int");
				}
			} else {
				error(child, "Incorrect operand type, expected string or int");
			}
		}
	} else if (node.type == "assert") {
		isBoolType(node.children.at(0));
	} else if (node.type == "add") {
		// check that adding two operands of the same type (int/string)
		std::string type1 = node.children.at(0).type;

		if (type1 == "ident") {
			identExists(node.children.at(0));
			type1 = symboltable.at(node.children.at(0).text).type;
		}

		if (type1 == "number" || type1 == "add" || type1 == "sub" || type1 == "mul" || type1 == "div" || type1 == "int") {
			isNumberType(node.children.at(1));
		} else if (type1 == "text" || type1 == "string") {
			isStringType(node.children.at(1));
		} else {
			error(node, "Unrecognizable operand type");
		}
	} else if (node.type == "sub" || node.type == "mul" || node.type == "div" || node.type == "less") {
		isNumberType(node.children.at(0));
		isNumberType(node.children.at(1));
	} else if (node.type == "and") {
		isBoolType(node.children.at(0));
		isBoolType(node.children.at(1));
	} else if (node.type == "equal" || node.type == "less") {
		// check that comparing two operands of the same type (int/bool/string)
		std::string type1 = node.children.at(0).type;

		if (type1 == "ident") {
			identExists(node.children.at(0));
			type1 = symboltable.at(node.children.at(0).text).type;
		}

		if (type1 == "number" || type1 == "add" || type1 == "sub" || type1 == "mul" || type1 == "div" || type1 == "int") {
			isNumberType(node.children.at(1));
		} else if (type1 == "bool" || type1 == "less" || type1 == "equal" || type1 == "not") {
			isBoolType(node.children.at(0));
		} else if (type1 == "text" || type1 == "string") {
			isStringType(node.children.at(1));
		} else {
			error(node, "Unrecognizable operand type");
		}
	} else if (node.type == "not") {
		isBoolType(node.children.at(0));
	} else if (node.type == "type") {
		// no checks
	} else if (node.type == "number") {
		// no checks
	} else if (node.type == "text") {
		// no checks
	} else if (node.type == "ident") {
		// no checks
	} else if (node.type == "bool") {
		// no checks
	}

	for (ASTnode child : node.children) {
		analyzeNode(child);
	}
}

void SemanticAnalyzer::checkOperCount(ASTnode node, int count) {
	if (node.children.size() != count) {
		error(node, "Incorrent number of operands");
	}
}

void SemanticAnalyzer::isNumberType(ASTnode node) {
	if (node.type == "number" ||
		node.type == "add" ||
		node.type == "sub" ||
		node.type == "mul" ||
		node.type == "div") {
			return;
	}

	if (node.type == "ident") {
		identExists(node);
		if (symboltable.at(node.text).type == "int") { return; }
	}

	error(node, "Incorrect operand type, expected int");
}

void SemanticAnalyzer::isBoolType(ASTnode node) {
	if (node.type == "bool" ||
		node.type == "less" ||
		node.type == "equal" ||
		node.type == "not") {
		return;
	}

	if (node.type == "ident") {
		identExists(node);
		if (symboltable.at(node.text).type == "bool") { return; }
	}

	error(node, "Incorrect operand type, expected bool");
}

void SemanticAnalyzer::isStringType(ASTnode node) {
	if (node.type == "text") {
		return;
	}

	if (node.type == "ident") {
		identExists(node);
		if (symboltable.at(node.text).type == "string") { return; }
	}

	error(node, "Incorrect operand type, expected string");
}

void SemanticAnalyzer::identExists(ASTnode node) {
	if (symboltable.find(node.text) != symboltable.end()) {
		return;
	}

	error(node, "Identifier must be initialized before use");
}