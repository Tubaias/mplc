#include "mplCompiler.h"

void printChars(std::vector<char> chars) {
	for (char c : chars) {
		std::cout << c;
	}

	std::cout << "\n\n----------\n" << std::endl;
}

void printAST(ASTnode ast) {
	std::cout << " (" << ast.type;

	if (ast.text != "") {
		std::cout << "[" << ast.text << "]";
	}

	for (ASTnode n : ast.children) {
		printAST(n);
	}
	std::cout << ")";
}

int main(int argc, char* argv[]) {
	std::cout << "Starting.\n" << std::endl;

	if (argc < 2) {
		std::cout << "Missing filename argument." << std::endl;
		return 1;
	}

	// read the file into a char vector
	std::string filename(argv[1]);
	FileReader reader(filename);
	std::vector<char> chars = reader.read();
	if (chars.empty()) {
		return 1;
	}

	printChars(chars); // EXTRA

	// scanning and parsing
	Parser parser(chars);
	parser.printTokens();
	ASTnode ast = parser.parse();

	printAST(ast); // EXTRA
	std::cout << "\n"; // EXTRA

	if (ast.text == "error") {
		return 0;
	}

	return 0;
}