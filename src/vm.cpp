#include "vm.hpp"

#include "compiler.hpp"

#ifdef DEBUG_TRACE_EXECUTION
#include "debug.hpp"
#endif

#define SWITCH \
	/* assert(IS_OBJ(currentThread->base[-3]) && (OBJ_TYPE(currentThread->base[-3]) == OBJ_CLOSURE));*/ \
	bytecode = READ_BYTECODE(); \
	switch(OP(bytecode))
#define DISPATCH continue
#define TARGET(op) case op
#define DEFAULT \
	default: \
	fprintf(stderr, "Unimplemented opcode %d.\n", OP(bytecode)); \
	return INTERPRET_RUNTIME_ERROR;
#define READ_BYTECODE() (*ip_++)
#define READ_CONSTANT() chunk->constants[RD(bytecode)];
#define BINARY_OP(op) \
	base[RA(bytecode)] = base[RA(bytecode)] op base[RA(bytecode)]

InterpretResult VM::run(void) {
	uint32_t bytecode;
	register uint32_t *ip_ = ip;

	while(true) {
#ifdef DEBUG_TRACE_EXECUTION
		disassembleInstruction(*chunk, ip - chunk->code.data());
#endif
		SWITCH {
			TARGET(OP_CONST_NUM): {
				Value constant = READ_CONSTANT();
				base[RA(bytecode)] = constant;
				DISPATCH;
			}
			TARGET(OP_NEGATE):
				base[RA(bytecode)] = -base[RD(bytecode)]; DISPATCH;
			TARGET(OP_ADD):
				BINARY_OP(+); DISPATCH;
			TARGET(OP_SUB):
				BINARY_OP(-); DISPATCH;
			TARGET(OP_MUL):
				BINARY_OP(*); DISPATCH;
			TARGET(OP_DIV):
				BINARY_OP(/); DISPATCH;
			TARGET(OP_RETURN):
				return INTERPRET_OK;
				DISPATCH;
			DEFAULT
		}
	}
}
#undef SWITCH
#undef DISPATCH
#undef TARGET
#undef DEFAULT
#undef READ_BYTECODE
#undef READ_CONSTANT

InterpretResult VM::interpret(std::istream &src) {
	compile(src);
	// ip = chunk->code.data();
	// return run();
	return INTERPRET_OK;
}
