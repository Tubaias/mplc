# CMakeList.txt : CMake project for mplCompiler, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("mplCompiler")

# Add source to this project's executable.
add_executable (mplCompiler "src/mplCompiler.cpp" "src/mplCompiler.h" "src/FileReader.h" "src/FileReader.cpp" "src/Scanner.h" "src/Scanner.cpp" "src/struct/Token.h" "src/Parser.h" "src/Parser.cpp" "src/struct/ASTnode.h" "src/SemanticAnalyzer.h" "src/SemanticAnalyzer.cpp" "src/struct/VarItem.h" "src/Interpreter.h" "src/Interpreter.cpp")

# TODO: Add tests and install targets if needed.
