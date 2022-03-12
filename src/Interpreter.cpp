#include "Interpreter.h"

Interpreter::Interpreter(std::unordered_map<std::string, VarItem>& st) {
	symbolTable = st;
}

// interpretation entry point
void Interpreter::interpret(ASTnode ast) {
	try {
		for (ASTnode statement : ast.children) {
			evalNode(statement);
		}
	} catch (std::exception e) {
		std::cout << "RUNTIME ERROR: " << e.what() << std::endl; // lazy error handling
	}
}

// main evaluation function
VarItem Interpreter::evalNode(ASTnode node) {
	VarItem v;
	v.type = "none";

	if (node.type == "var") {
		if (node.children.size() == 3) {
			VarItem operand = evalNode(node.children.at(2));
			symbolTable[node.children.at(0).text] = operand;
		}
	} else if (node.type == "assign") {
		VarItem operand = evalNode(node.children.at(1));
		symbolTable[node.children.at(0).text] = operand;
	} else if (node.type == "loop") {
		handleLoop(node);
	} else if (node.type == "read") {
		handleRead(node);
	} else if (node.type == "print") {
		VarItem operand = evalNode(node.children.at(0));
		std::cout << operand.value;
	} else if (node.type == "assert") {
		VarItem operand = evalNode(node.children.at(0));
		if (operand.value == "false") {
			std::cout << "Assertion failed." << std::endl; // this could be better
		}
	} else if (node.type == "add") {
		v = evalAdd(node);
	} else if (node.type == "sub" || node.type == "mul" || node.type == "div") {
		v = evalIntOp(node);
	} else if (node.type == "and") {
		v = evalAnd(node);
	} else if (node.type == "equal") {
		v = evalEqual(node);
	} else if (node.type == "less") {
		v = evalLess(node);
	} else if (node.type == "not") {
		v = evalNot(node);
	} else if (node.type == "type") {
		// do nothing
	} else if (node.type == "number") {
		v.type = "int";
		v.value = node.text;
	} else if (node.type == "text") {
		v.type = "string";
		v.value = node.text;
	} else if (node.type == "bool") {
		v.type = "bool";
		v.value = node.text;
	} else if (node.type == "ident") {
		v = symbolTable.at(node.text);
	}

	return v;
}

void Interpreter::handleLoop(ASTnode node) {
	std::string control = node.children.at(0).text;
	VarItem startPoint = evalNode(node.children.at(1));
	VarItem endPoint = evalNode(node.children.at(2));
	symbolTable.at(control).value = startPoint.value;
	int controlVal = std::stoi(startPoint.value);
	int endVal = std::stoi(endPoint.value);

	while (controlVal <= endVal) {
		for (int i = 3; i < node.children.size(); i++) {
			evalNode(node.children.at(i));
		}

		controlVal++;
		symbolTable.at(control).value = std::to_string(controlVal);
	}
}

void Interpreter::handleRead(ASTnode node) {
	std::string identName = node.children.at(0).text;

	std::string input;
	std::cin >> input;

	if (symbolTable.at(identName).type == "int") {
		try {
			int value = std::stoi(input);
			symbolTable.at(identName).value = std::to_string(value);
		} catch (std::exception e) {
			throw std::exception("Could not parse input into integer");
		}
	} else {
		symbolTable.at(identName).value = input;
	}
}

VarItem Interpreter::evalAdd(ASTnode node) {
	VarItem v;
	VarItem operand1 = evalNode(node.children.at(0));
	VarItem operand2 = evalNode(node.children.at(1));

	if (operand1.type == "int") {
		v.type = "int";
		int val1 = std::stoi(operand1.value);
		int val2 = std::stoi(operand2.value);
		v.value = std::to_string(val1 + val2);
	} else {
		v.type = "string";
		v.value = operand1.value + operand2.value;
	}

	return v;
}

VarItem Interpreter::evalIntOp(ASTnode node) {
	VarItem v;
	v.type = "int";
	VarItem operand1 = evalNode(node.children.at(0));
	VarItem operand2 = evalNode(node.children.at(1));
	int val1 = std::stoi(operand1.value);
	int val2 = std::stoi(operand2.value);

	int res = 0;
	if (node.type == "sub") {
		res = val1 - val2;
	} else if (node.type == "mul") {
		res = val1 * val2;
	} else {
		res = val1 / val2;
	}
	
	v.value = std::to_string(res);
	return v;
}

VarItem Interpreter::evalAnd(ASTnode node) {
	VarItem v;
	v.type = "bool";
	VarItem operand1 = evalNode(node.children.at(0));
	VarItem operand2 = evalNode(node.children.at(1));

	if (operand1.value == "true" && operand2.value == "true") {
		v.value = "true";
	} else {
		v.value = "false";
	}

	return v;
}

VarItem Interpreter::evalEqual(ASTnode node) {
	VarItem v;
	v.type = "bool";
	VarItem operand1 = evalNode(node.children.at(0));
	VarItem operand2 = evalNode(node.children.at(1));

	if (operand1.value == operand2.value) {
		v.value = "true";
	} else {
		v.value = "false";
	}

	return v;
}

VarItem Interpreter::evalLess(ASTnode node) {
	VarItem v;
	v.type = "bool";
	VarItem operand1 = evalNode(node.children.at(0));
	VarItem operand2 = evalNode(node.children.at(1));

	if (operand1.type == "int") {
		int val1 = std::stoi(operand1.value);
		int val2 = std::stoi(operand2.value);
		if (val1 < val2) {
			v.value = "true";
		} else {
			v.value = "false";
		}
	} else if (operand1.type == "string") {
		if (operand1.value < operand2.value) { // lexicographical ordering
			v.value = "true";
		} else {
			v.value = "false";
		}
	} else {
		if (operand1.value == "false" && operand2.value == "true") { // false = 0, true = 1
			v.value = "true";
		} else {
			v.value = "false";
		}
	}

	return v;
}

VarItem Interpreter::evalNot(ASTnode node) {
	VarItem v;
	v.type = "bool";
	VarItem operand = evalNode(node.children.at(0));

	if (operand.value == "false") {
		v.value = "true";
	} else {
		v.value = "false";
	}

	return v;
}