#ifndef TLOX_VM_HPP
#define TLOX_VM_HPP

#include "tlox.hpp"
#include "chunk.hpp"

#include <istream>

class VM {
	public:
		VM(void) : stack(16, 0), base(stack.data())
			{};
		InterpretResult interpret(std::istream &src);
	private:
		InterpretResult run(void);

		Chunk *chunk;
		uint32_t *ip;
		std::vector<Value, alloc<Value>> stack;
		Value *base;
};

#endif /* TLOX_VM_HPP */
