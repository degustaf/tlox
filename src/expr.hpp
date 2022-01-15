#ifndef TLOX_EXPR_HPP
#define TLOX_EXPR_HPP

#include "common.hpp"
#include "scanner.hpp"
#include "value.hpp"

#include <memory>
#include <string>

class ExprVisitor;

struct Expr {
	virtual ~Expr() = default;
	virtual std::string accept(const ExprVisitor &visitor) const = 0;
};

#define comma ,
#define TypeBuilder(tp, nm) tp nm
#define InitBuilder(tp, nm) nm(nm)

#define StructBuilder(name, builder) \
	struct name##Expr : Expr { \
		name##Expr(builder(TypeBuilder, comma)) : builder(InitBuilder, comma) {}; \
		builder(TypeBuilder, ;); \
		virtual std::string accept(const ExprVisitor &visitor) const override; \
	}

#define BinaryBuilder(X, sep) \
	X(std::shared_ptr<Expr>, left)sep \
	X(Token, op)sep \
	X(std::shared_ptr<Expr>, right)

#define GroupingBuilder(X, sep) \
	X(std::shared_ptr<Expr>, exp)

#define LiteralBuilder(X, sep) \
	X(Value, value)

#define UnaryBuilder(X, sep) \
	X(Token, op)sep \
	X(std::shared_ptr<Expr>, right)

#define sBuilder(X) StructBuilder(X, X##Builder)

#define ExprBuilder(X, sep) \
	X(Binary)sep \
	X(Grouping)sep \
	X(Literal)sep \
	X(Unary)

ExprBuilder(sBuilder, ;);

#define visitBuilder(tp) \
	virtual std::string visit(const tp##Expr&) const = 0

struct ExprVisitor {
	ExprBuilder(visitBuilder, ;);
};

#endif /* TLOX_EXPR_HPP */
