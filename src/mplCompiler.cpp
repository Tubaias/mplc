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
	bool verbose = false;

	if (argc < 2) {
		std::cout << "Missing filename argument." << std::endl;
		return 1;
	} else if (argc > 2) {
		if (std::strcmp(argv[2], "-v") == 0) {
			verbose = true;
		}
	}

	if (verbose) { std::cout << "Starting in verbose mode.\n" << std::endl; }

	// read the file into a char vector
	std::string filename(argv[1]);
	FileReader reader(filename);
	std::vector<char> chars = reader.read();
	if (chars.empty()) {
		return 1;
	}

	if (verbose) { printChars(chars); }

	// scanning and parsing
	Parser parser(chars);
	if (verbose) {
		std::cout << "TOKENS:\n";
		parser.printTokens();
	}
	
	ASTnode ast = parser.parse();

	if (verbose) {
		std::cout << "SYNTAX TREE:\n";
		printAST(ast);
		std::cout << "\n\n";
	}

	if (ast.text == "error") {
		return 0;
	}

	// semantic analysis
	SemanticAnalyzer sa;
	std::unordered_map<std::string, VarItem> symbolTable = sa.analyze(ast);

	if (symbolTable.find("") != symbolTable.end()) {
		return 0;
	}

	if (verbose) {
		std::cout << "OUTPUT:\n";
	}

	// interpretation
	Interpreter inter(symbolTable);
	inter.interpret(ast);
	return 0;
}