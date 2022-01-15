#ifndef TLOX_DEBUG_HPP
#define TLOX_DEBUG_HPP

#include <string>

#include "chunk.hpp"

void disassembleInstruction(const Chunk &chunk, size_t offset);
void disassembleChunk(const Chunk &chunk, const std::string &name);

#endif /* TLOX_DEBUG_HPP */
