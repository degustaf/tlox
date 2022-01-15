#ifndef TLOX_CHUNK_HPP
#define TLOX_CHUNK_HPP

#include <cstdint>
#include <vector>

#include "common.hpp"
#include "type.hpp"
#include "memory.hpp"

#define COMMA ,
#define OPCODE_BUILDER(X, sep) \
	X(OP_CONST_NUM,			ADConst)sep		/*  0 */ \
	X(OP_NEGATE,			AD)sep \
	X(OP_ADD,				ABC)sep \
	X(OP_SUB,				ABC)sep \
	X(OP_MUL,				ABC)sep \
	X(OP_DIV,				ABC)sep \
	X(OP_RETURN,			ADret)sep
#define BUILD_OPCODES(op, _) op

enum OpCode {
	OPCODE_BUILDER(BUILD_OPCODES, COMMA)
};

// These macros are designed to put data into the uint32_t bytecodes.
#define OP_A(op,a) (((uint32_t) (op)) | (((uint32_t) (a)) << 8))
#define OP_D(op,d) (((uint32_t) (op)) | (((uint32_t) (d)) << 16))
#define OP_AD(op,a,d) (OP_A((op),(a)) | (((uint32_t) (d)) << 16))
#define OP_ABC(op,a,b,c) OP_AD((op), (a), (((uint16_t) (b)) | (((uint16_t) (c)) << 8)))
#define OP_AJump(op,a,j) OP_AD((op), (a), (OP_position)((int32_t)(j)+JUMP_BIAS))

#define MAX_REG 0xff
#define MAX_D 0xffff
#define JUMP_BIAS 0x8000
#define NO_REG MAX_REG
#define NO_JUMP (~(OP_position)0)

// These macros are designed to pull data out of the uint32_t bytecodes.
#define OP(x) ((Reg)(MAX_REG & ((uint32_t)(x))))
#define RA(x) ((Reg)(((uint16_t)(MAX_D & ((uint32_t)(x)))) >> 8))
#define RD(x) ((uint16_t)((x) >> 16))
#define RJump(x) ((ptrdiff_t)RD(x)-JUMP_BIAS)
#define RB(x) ((Reg)(MAX_REG & RD(x)))
#define RC(x) ((Reg)(RD(x) >> 8))

class Chunk {
	public:
		Chunk(void) {};
		void write(uint32_t byte, size_t line);
		size_t addConstant(Value value);

		std::vector<uint32_t, alloc<uint32_t>> code;
		std::vector<Value, alloc<Value>> constants;
		std::vector<size_t, alloc<size_t>> lines;
	private:
};

#endif /* TLOX_CHUNK_HPP */
