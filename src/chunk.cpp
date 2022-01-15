#include "chunk.hpp"

void Chunk::write(uint32_t byte, size_t line) {
	code.push_back(byte);
	lines.push_back(line);
}

size_t Chunk::addConstant(Value value) {
	constants.push_back(value);
	return constants.size() - 1;
}
