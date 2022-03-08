#include "Scanner.h"

#include <cctype>

Scanner::Scanner(std::vector<char> characters) {
	keywords = {"var", "for", "end", "in", "do", "read", "print", "int", "string", "bool", "assert", "true", "false"};
	chars = characters;
	it = chars.begin();
}

char Scanner::nextChar() {
	if (it == chars.end()) {
		return 0;
	}

	char next = *it;
	it++;
	return next;
}

Token Scanner::scanToken() {
	Token t;
	std::string text = "";
	char c = nextChar();

	// throw away whitespace
	while (isspace(c)) {
		c = nextChar();
	}

	// check that file hasn't ended
	if (c == 0) {
		t.type = "END"; return t;
	}

	switch (c) {
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
		case '<':
			t.type = "less"; return t;
		case '=':
			t.type = "equal"; return t;
		case '&':
			t.type = "and"; return t;
		case '!':
			t.type = "not"; return t;
		// dot
		case '.':
			c = nextChar();
			if (c == 0) {
				t.type = "ENDERR"; return t;
			}

			if (c == '.') {
				t.type = "dots"; return t;
			} else {
				t.type = "ERROR";
				t.text = "Unexpected character following '.'";
				return t;
			}
		// colon
		case ':':
			c = nextChar();
			if (c == 0) {
				t.type = "ENDERR"; return t;
			}

			if (c == '=') {
				t.type = "assign"; return t;
			} else {
				t.type = "col"; return t;
			}
		// forward slash
		case '/':
			c = nextChar();
			if (c == 0) {
				t.type = "ENDERR"; return t;
			}

			if (c == '/') { // single line comment
				while (true) {
					c = nextChar();
					if (c == 0) {
						t.type = "END"; return t;
					}

					if (c == 10 /* line feed */) {
						return scanToken();
					}
				}
			} else if (c == '*') { // comment block
				while (true) {
					c = nextChar();
					if (c == 0) {
						t.type = "END"; return t;
					}

					if (c == '*') {
						c = nextChar();
						if (c == '/') {
							return scanToken();
						}
					}
				}
			} else {
				t.type = "div"; return t;
			}
		// quote
		case '"':
			while (true) {
				c = nextChar();
				if (c == 0) {
					t.type = "ENDERR"; return t;
				}

				if (c == '"') {
					t.type = "text";
					t.text = text;
					return t;
				} else {
					text.push_back(c);
				}
			}
		default:
			break;
	}

	// numbers
	if (isdigit(c)) {
		while (true) {
			text.push_back(c);

			c = nextChar();
			if (c == 0) {
				t.type = "ENDERR"; return t;
			}

			if (!isdigit(c)) {
				it--;
				t.type = "number";
				t.text = text;
				return t;
			}
		}
	}
	
	// letters
	if (isalpha(c)) { 
		while (true) {
			text.push_back(c);

			c = nextChar();
			if (c == 0) {
				t.type = "ENDERR"; return t;
			}

			if (!isalnum(c) && c != '_') {
				it--;
				if (keywords.find(text) != keywords.end()) {
					t.type = text; // keyword
					return t;
				} else {
					t.type = "ident"; // identifier
					t.text = text;
					return t;
				}
			}
		}
	}
}