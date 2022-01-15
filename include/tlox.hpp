#ifndef TLOX_TLOX_HPP
#define TLOX_TLOX_HPP

#include <istream>

enum InterpretResult {
	INTERPRET_OK,
	INTERPRET_COMPILE_ERROR,
	INTERPRET_RUNTIME_ERROR
};

// vm.hpp imports this file to define InterpretResult, so we can't include it until after that's defined.
#include "vm.hpp"

InterpretResult interpret(std::istream &src);

#endif /* TLOX_TLOX_HPP */
