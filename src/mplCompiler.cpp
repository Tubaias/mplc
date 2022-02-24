#include "mplCompiler.h"

int main() {
	std::cout << "Starting.\n" << std::endl;

	// read the file into a char vector
	FileReader reader("sample1.mpl");
	std::vector<char> chars = reader.read();

	if (chars.empty()) {
		return 1;
	}

	// scanning
	Scanner scanner(chars);
	scanner.scan();

	return 0;
}