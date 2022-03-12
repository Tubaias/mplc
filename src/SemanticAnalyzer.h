#pragma once

#include <unordered_map>
#include <string>
#include <iostream>
#include "ASTnode.h"
#include "VarItem.h"

class SemanticException : public std::exception {
public:
	SemanticException(std::string message) {
		msg = message;
	};

	virtual const char* what() const throw() {
		return msg.c_str();
	}

private:
	std::string msg;
};

class SemanticAnalyzer {
	public:
		SemanticAnalyzer();
		std::unordered_map<std::string, VarItem> analyze(ASTnode ast);
	private:
		std::unordered_map<std::string, VarItem> symboltable;
		bool hadError;

		void error(ASTnode node, std::string msg);
		void handleException(SemanticException e);
		void printSymboltable();
		void analyzeNode(ASTnode ast);

		void checkOperCount(ASTnode node, int count);
		void isNumberType(ASTnode node);
		void isBoolType(ASTnode node);
		void isStringType(ASTnode node);
		void identExists(ASTnode node);
};