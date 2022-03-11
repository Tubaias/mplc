#include "Parser.h"

#include <iostream>

Parser::Parser(std::vector<char> characters) : scanner(characters) {
	hadError = false;
	endError = false;
}

// printing for diagnostics
void Parser::printTokens() {
	while (true) {
		Token t = scanner.scanToken();
		if (t.type == "END") {
			break;
		} else if (t.type == "ERREND") {
			std::cout << t.text << std::endl;
			break;
		}

		if (t.text == "") {
			std::cout << t.type << std::endl;
		} else {
			std::cout << t.type << "(" << t.text << ")" << std::endl;
		}
	}

	std::cout << "\n";
	scanner.reset();
}

// parse error handling and recovery
void Parser::error(Token t, std::string expectation) noexcept(false) {
	hadError = true;

	std::string msg = "";
	if (t.text == "") {
		msg += "expected '" + expectation + "' but got '" + t.type + "'";
	} else {
		msg += "expected '" + expectation + "' but got '" + t.type + "(" + t.text + ")'";
	}

	msg += " (Line: " + std::to_string(t.line) + ")";

	if (current.type != "END" && current.type != "ENDERR") {
		while (!isAtEnd() && !check("semicol")) {
			advance();
		}

		advance();
	}

	throw ParserException(msg);
}

// error message printing
void Parser::handleException(ParserException e) {
	std::cout << "PARSE ERROR: " << e.what() << std::endl;
}

// check if we are out of tokens
bool Parser::isAtEnd() {
	if (next.type == "ENDERR") {
		hadError = true;
		endError = true;
		return true;
	}

	return (next.type == "END");
}

// consume a token
void Parser::advance() {
	if (!isAtEnd()) {
		current = next;
		next = scanner.scanToken();
	}
}

// check if current matches type(s)
bool Parser::check(std::string type) {
	if (isAtEnd()) return false;
	return next.type == type;
}

bool Parser::check(std::vector<std::string> types) {
	if (isAtEnd()) return false;

	for (std::string type : types) {
		if (next.type == type) {
			return true;
		}
	}

	return false;
}

// check if current matches type(s), consume if true, error if false
bool Parser::match(std::string type) {
	if (check(type)) {
		advance();
		return true;
	}

	error(next, type);
	return false;
}

bool Parser::match(std::vector<std::string> types) {
	if (check(types)) {
		advance();
		return true;
	}

	std::string expectation = "";
	for (std::string type : types) {
		expectation += type;
		expectation += "/";
	}

	expectation.pop_back();
	error(next, expectation);
	return false;
}

/*
<prog> ::= <stmt> ";" ( <stmt> ";" )*
<stmt> ::= "var" <ident> ":" <type> [ ":=" <expr> ]
		| <ident> ":=" <expr>
		| "for" <ident> "in" <expr> ".." <expr> "do" ( <stmt> ";" )* "end" "for"
		| "read" <ident>
		| "print" <expr>
		| "assert" "(" <expr> ")"
<expr> ::= <bin_expr>
		| <un_expr>
<bin_expr> ::= <opnd> <op> <opnd>
<un_expr> ::= [ <unary_op> ] <opnd>
<opnd> ::= <int>
		| <string>
		| "true"
		| "false"
		| <ident>
		| "(" <expr> ")"
<type> ::= "int"
		| "string"
		| "bool"
*/

std::vector<std::string> stmt_tokens = { "var", "ident", "for", "read", "print", "assert" };
std::vector<std::string> op_tokens = { "add", "sub", "mul", "div", "less", "equal", "and" };
std::vector<std::string> type_tokens = { "int", "string", "bool" };

// recursive descent parsing entry point
ASTnode Parser::parse() {
	ASTnode program;
	program.type = "program";
	next = scanner.scanToken();

	// first statement (mandatory)
	try {
		match(stmt_tokens);
		program.children.push_back(stmt());
		match("semicol");
	} catch (ParserException e) {
		handleException(e);
	}

	// following statements
	try {
		while (check(stmt_tokens)) {
			try {
				advance();
				program.children.push_back(stmt());
				match("semicol");
			} catch (ParserException e) {
				handleException(e);
			}
		}
	} catch (ParserException e) {
		handleException(e);
	}

	// scanner ran out of input unexpectedly
	if (endError) {
		hadError = true;
		ParserException e("Ran out of input unexpectedly.");
		handleException(e);
	}

	// something at the end of the file that isn't a statement
	if (!hadError && !isAtEnd()) {
		hadError = true;
		ParserException e("Unknown token(s) at the end of input.");
		handleException(e);
	}

	if (hadError) {
		program.text = "error";
	}

	return program;
}

ASTnode Parser::stmt() {
	if (current.type == "var") {
		return variable();
	} else if (current.type == "ident") {
		return assign();
	} else if (current.type == "for") {
		return loop();
	} else if (current.type == "read") {
		return read();
	} else if (current.type == "print") {
		return print();
	} else if (current.type == "assert") {
		return assert();
	}

	ASTnode node;
	return node;
}

ASTnode Parser::variable() {
	ASTnode node;
	node.type = "var";

	match("ident");
	node.children.push_back(identifier());

	match("col");
	match(type_tokens);
	node.children.push_back(type());

	if (check("assign")) {
		advance();
		node.children.push_back(expression());
	}

	return node;
}

ASTnode Parser::type() {
	ASTnode node;
	node.type = "type";
	node.text = current.type;
	return node;
}

ASTnode Parser::assign() {
	ASTnode node;
	node.type = "assign";
	node.children.push_back(identifier());

	match("assign");

	node.children.push_back(expression());
	return node;
}

ASTnode Parser::loop() {
	ASTnode node;
	node.type = "loop";

	match("ident");
	node.children.push_back(identifier());
	
	match("in");
	node.children.push_back(expression());
	match("dots");
	node.children.push_back(expression());
	match("do");

	while (check(stmt_tokens)) {
		advance();
		node.children.push_back(stmt());
		match("semicol");
	}

	match("end");
	match("for");

	return node;
}

ASTnode Parser::read() {
	ASTnode node;
	node.type = "read";

	match("ident");
	node.children.push_back(identifier());
	return node;
}

ASTnode Parser::print() {
	ASTnode node;
	node.type = "print";

	node.children.push_back(expression());
	return node;
}

ASTnode Parser::assert() {
	ASTnode node;
	node.type = "assert";

	match("lbrack");
	node.children.push_back(expression());
	match("rbrack");
	return node;
}

ASTnode Parser::expression() {
	advance();
	
	// look ahead to see if we have an operator
	if (check(op_tokens)) {
		return binaryExpr();
	} else {
		return unaryExpr();
	}
}

ASTnode Parser::binaryExpr() {
	ASTnode node;
	node.children.push_back(operand());
	advance();
	node.type = current.type;
	advance();
	node.children.push_back(operand());
	return node;
}

ASTnode Parser::unaryExpr() {
	if (current.type == "not") {
		ASTnode node;
		node.type = "not";
		node.children.push_back(operand());
		return node;
	} else {
		return operand();
	}
}

ASTnode Parser::operand() {
	ASTnode node;

	if (current.type == "number" || current.type == "text" || current.type == "ident") {
		node.type = current.type;
		node.text = current.text;
	} else if (current.type == "true" || current.type == "false") {
		node.type = "bool";
		node.text = current.type;
	} else if (current.type == "lbrack") {
		node = expression();
		match("rbrack");
	} else {
		error(current, "operand");
	}

	return node;
}

ASTnode Parser::identifier() {
	ASTnode ident;
	ident.type = "ident";
	ident.text = current.text;
	return ident;
}