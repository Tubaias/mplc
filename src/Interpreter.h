#pragma once

#include <unordered_map>
#include <iostream>
#include "struct/ASTnode.h"
#include "struct/VarItem.h"

class RuntimeException : public std::exception {
public:
	RuntimeException(std::string message) {
		msg = message;
	};

	virtual const char* what() const throw() {
		return msg.c_str();
	}

private:
	std::string msg;
};

class Interpreter {
	public:
		Interpreter(std::unordered_map<std::string, VarItem>& st);
		void interpret(ASTnode ast);
	private:
		std::unordered_map<std::string, VarItem> symbolTable;
		VarItem evalNode(ASTnode node);

		void handleLoop(ASTnode node);
		void handleRead(ASTnode node);
		VarItem evalAdd(ASTnode node);
		VarItem evalIntOp(ASTnode node);
		VarItem evalAnd(ASTnode node);
		VarItem evalEqual(ASTnode node);
		VarItem evalLess(ASTnode node);
		VarItem evalNot(ASTnode node);
};