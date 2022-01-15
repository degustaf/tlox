#include "compiler.hpp"

#include "scanner.hpp"

void compile(std::istream &src) {
	Scanner scanner(src);
	size_t line = (size_t)(-1);

	while(true) {
		Token token;
		scanner >> token;
		if(token.line != line) {
			printf("%4zu ", token.line);
			line = token.line;
		} else {
			printf("   | ");
		}

		printf("%2d '%s'\n", token.type, token.str.c_str()); 
		if (token.type == TOKEN_EOF) break;
	}
}
