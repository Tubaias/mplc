#include "mplCompiler.h"

int main(int argc, char* argv[]) {
	std::cout << "Starting.\n" << std::endl;

	if (argc < 2) {
		std::cout << "Missing filename argument." << std::endl;
		return 1;
	}

	std::string filename(argv[1]);

	// read the file into a char vector
	FileReader reader(filename);
	std::vector<char> chars = reader.read();

	if (chars.empty()) {
		return 1;
	}

	// ------ PRINTING ----------------------
	for (char c : chars) {
		std::cout << c;
	}

	std::cout << "\n\n----------\n" << std::endl;
	// ------ PRINTING ----------------------

	// scanning and parsing
	Parser parser(chars);
	parser.parse();

	return 0;
}