#include "Scanner.h"

#include <cctype>

Scanner::Scanner(std::vector<char> characters) {
	keywords = {"var", "for", "end", "in", "do", "read", "print", "int", "string", "bool", "assert", "true", "false"};
	chars = characters;
	it = chars.begin();
	line = 1;
}

void Scanner::reset() {
	it = chars.begin();
	line = 1;
}

void Scanner::stepBack() {
	it--;
	if (*it == 10 /* line feed */) line--;
}

char Scanner::nextChar() {
	if (it == chars.end()) {
		return 0;
	}

	char next = *it;
	it++;

	if (next == 10 /* line feed */) line++;

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
			t.type = "add"; t.line = line; return t;
		case '-':
			t.type = "sub"; t.line = line; return t;
		case '*':
			t.type = "mul"; t.line = line; return t;
		case '(':
			t.type = "lbrack"; t.line = line; return t;
		case ')':
			t.type = "rbrack"; t.line = line; return t;
		case ';':
			t.type = "semicol"; t.line = line; return t;
		case '<':
			t.type = "less"; t.line = line; return t;
		case '=':
			t.type = "equal"; t.line = line; return t;
		case '&':
			t.type = "and"; t.line = line; return t;
		case '!':
			t.type = "not"; t.line = line; return t;
		// dot
		case '.':
			c = nextChar();
			if (c == 0) {
				t.type = "ENDERR"; t.line = line; return t;
			}

			if (c == '.') {
				t.type = "dots"; t.line = line; return t;
			} else {
				t.type = "INVALID INPUT";
				t.text = "Unexpected character following '.'";
				t.line = line;
				return t;
			}
		// colon
		case ':':
			c = nextChar();
			if (c == 0) {
				t.type = "ENDERR"; t.line = line; return t;
			}

			if (c == '=') {
				t.type = "assign"; t.line = line; return t;
			} else {
				stepBack();
				t.type = "col"; t.line = line; return t;
			}
		// forward slash
		case '/':
			c = nextChar();
			if (c == 0) {
				t.type = "ENDERR"; t.line = line; return t;
			}

			if (c == '/') { // single line comment
				while (true) {
					c = nextChar();
					if (c == 0) {
						t.type = "END"; t.line = line; return t;
					}

					if (c == 10 /* line feed */) {
						return scanToken();
					}
				}
			} else if (c == '*') { // comment block
				while (true) {
					c = nextChar();
					if (c == 0) {
						t.type = "ENDERR"; t.line = line; return t;
					}

					if (c == '*') {
						c = nextChar();
						if (c == '/') {
							return scanToken();
						}
					}
				}
			} else {
				stepBack();
				t.type = "div"; t.line = line; return t;
			}
		// quote
		case '"':
			while (true) {
				c = nextChar();
				if (c == 0) {
					t.type = "ENDERR"; t.line = line; return t;
				}

				if (c == '"') {
					t.type = "text";
					t.text = text;
					t.line = line;
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
				t.type = "ENDERR"; t.line = line; return t;
			}

			if (!isdigit(c)) {
				stepBack();
				t.type = "number";
				t.text = text;
				t.line = line;
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
				t.type = "ENDERR"; t.line = line; return t;
			}

			if (!isalnum(c) && c != '_') {
				stepBack();
				if (keywords.find(text) != keywords.end()) {
					t.type = text; // keyword
					t.line = line;
					return t;
				} else {
					t.type = "ident"; // identifier
					t.text = text;
					t.line = line;
					return t;
				}
			}
		}
	}
}