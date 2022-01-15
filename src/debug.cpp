#include "debug.hpp"

#include <string>

#include "value.hpp"

using std::string;

static void InstructionABC(const string &name, __attribute__((unused)) const Chunk &chunk, uint32_t bytecode) {
	uint8_t regA = RA(bytecode);
	uint8_t regB = RB(bytecode);
	uint8_t regC = RC(bytecode);
	printf("%-16s Reg %4d Reg %4d -> Reg %4d\n", name.c_str(), regB, regC, regA);
}

static void InstructionAD(const string &name, __attribute__((unused)) const Chunk &chunk, uint32_t bytecode) {
	uint8_t reg = RA(bytecode);
	uint16_t dest = RD(bytecode);
	printf("%-16s register %4d to register %d\n", name.c_str(), dest, reg);
}

static void InstructionADConst(const string &name, const Chunk &chunk, uint32_t bytecode) {
	uint8_t reg = RA(bytecode);
	uint16_t constant = RD(bytecode);
	printf("%-16s %4d '", name.c_str(), constant);
	if(constant < chunk.constants.size())
		printValue(chunk.constants[constant]);
	else
		printf("Out of Range");
	printf("' to register %d\n", reg);
}

static void InstructionADret(const string &name, __attribute__((unused)) const Chunk &chunk, uint32_t bytecode) {
	uint8_t reg = RA(bytecode);
	uint16_t count = RD(bytecode);
	printf("%-16s return %4d - 1 registers starting at %d\n", name.c_str(), count, reg);
}

void disassembleInstruction(const Chunk &chunk, size_t offset) {
	uint32_t bytecode = chunk.code[offset];
	printf("%04zu %08x ", offset, bytecode);

	if(offset > 0 && chunk.lines[offset] == chunk.lines[offset - 1]) {
		printf("   | ");
	} else {
		printf("%4zu ", chunk.lines[offset]);
	}

#define BUILD_DISASSEMBLY(op, type) \
	case op: \
		Instruction##type(#op, chunk, bytecode); \
		break

	switch(OP(bytecode)) {
		OPCODE_BUILDER(BUILD_DISASSEMBLY, ;)
		default:
			printf("Unknown opcode %d\n", OP(bytecode));
			return;
	}
}

void disassembleChunk(const Chunk &chunk, const std::string &name) {
	printf("== %s ==", name.c_str());

	for(size_t offset=0; offset<chunk.code.size(); offset++) {
		disassembleInstruction(chunk, offset);
	}

}
