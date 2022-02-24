#include "Scanner.h"

#include <cctype>

Scanner::Scanner(std::vector<char> characters) {
	chars = characters;
	it = chars.begin();
}

Token Scanner::scanToken() {
	Token t;
	char currentChar = *it;
	it++;

	switch (currentChar) {
		// single characters
		case '+':
			t.type = "add"; return t;
		case '-':
			t.type = "sub"; return t;
		case '*':
			t.type = "mul"; return t;
		case '(':
			t.type = "lbrack"; return t;
		case ')':
			t.type = "rbrack"; return t;
		case ';':
			t.type = "semicol"; return t;
		// colon
		case ':':
			char next = *it;
			it++;
			if (next == '=') {
				t.type = "assign"; return t;
			} else if (isspace(next)) {
				t.type = "col"; return t;
			} else {
				t.type = "ERROR";
				t.text = "Super smart error message.";
				return t;
			}
		// forward slash
		case '/':
			char next = *it;
			it++;
			if (next == '/') {
				// handle comment
				return t;
			} else if (next == '*') {
				// handle comment
				return t;
			} else {
				t.type = "div"; return t;
			}
		default:
			break;
	}

	//numbers
	if (isdigit(currentChar)) {

	}
}