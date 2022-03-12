#pragma once

#include <exception>
#include "Scanner.h"
#include "struct/ASTnode.h"

class ParserException : public std::exception {
	public:
		ParserException(std::string message) {
			msg = message;
		};

		virtual const char* what() const throw() {
			return msg.c_str();
		}

	private:
		std::string msg;
};

class Parser {
	public:
		Parser(std::vector<char> characters);
		void printTokens();
		ASTnode parse();

	private:
		Scanner scanner;
		Token current;
		Token next;
		bool hadError;
		bool endError;

		void error(Token t, std::string expectation);
		void handleException(ParserException e);

		bool isAtEnd();
		void advance();
		bool check(std::string type);
		bool check(std::vector<std::string> types);
		bool match(std::string type);
		bool match(std::vector<std::string> types);

		ASTnode stmt();
		ASTnode variable();
		ASTnode type();
		ASTnode assign();
		ASTnode loop();
		ASTnode read();
		ASTnode print();
		ASTnode assert();
		ASTnode expression();
		ASTnode binaryExpr();
		ASTnode unaryExpr();
		ASTnode operand();
		ASTnode identifier();
};