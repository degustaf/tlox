#include "tlox.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include <assert.h>
#include <sysexits.h>

using std::cout;
using std::string;

static int runFile(const char *fileName) {
	std::ifstream src(fileName);
	VM vm;
	InterpretResult result = vm.interpret(src);
	if(result == INTERPRET_OK) {
		return 0;
	} else if(result == INTERPRET_COMPILE_ERROR) {
		return EX_DATAERR;
	} else if(result == INTERPRET_RUNTIME_ERROR) {
		return EX_SOFTWARE;
	}
	assert(false);
}

static int runPrompt(void) {
	VM vm;
	while(true) {
		cout << "> ";
		string line;
		if(!std::getline(std::cin, line))
			break;
		std::istringstream src(line);
		vm.interpret(src);
	}
	return 0;
}

int main(int argc, char* argv[]) {
	assert(argc > 1);
	if(argc > 2) {
		cout << "Usage: tlox [script]";
		return EX_USAGE;
	} else if(argc == 2) {
		return runFile(argv[1]);
	} else {
		return runPrompt();
	}
}
