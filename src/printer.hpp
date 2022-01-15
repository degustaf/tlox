#ifndef TLOX_PRINTER_HPP
#define TLOX_PRINTER_HPP

#include "expr.hpp"

#include <vector>

#undef visitBuilder
#define visitBuilder(tp) \
	virtual std::string visit(const tp##Expr&) const override

struct AstPrinter : ExprVisitor {
	std::string print(const Expr &e);
	ExprBuilder(visitBuilder, ;);
	private:
		std::string parenthesize(const std::string &name, const Expr &e) const;
		std::string parenthesize(const std::string &name, const Expr &e1, const Expr &e2) const;
};

#endif /* TLOX_PRINTER_HPP */
