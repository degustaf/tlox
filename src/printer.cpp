#include "printer.hpp"

using std::shared_ptr;
using std::string;
using std::vector;

string AstPrinter::parenthesize(const string &name, const Expr &e) const {
	string ret("(");
	ret.append(name);
	ret.append(" ").append(e.accept(*this));
	return ret.append(")");
}

string AstPrinter::parenthesize(const string &name, const Expr &e1, const Expr &e2) const {
	string ret("(");
	ret.append(name);
	ret.append(" ").append(e1.accept(*this));
	ret.append(" ").append(e2.accept(*this));
	return ret.append(")");
}

string AstPrinter::print(const Expr &e) {
	return e.accept(*this);
}

string AstPrinter::visit(const BinaryExpr &e) const {
	return parenthesize(e.op.str, *e.left, *e.right);
}

string AstPrinter::visit(const GroupingExpr &e) const {
	return parenthesize("group", *e.exp);
}

string AstPrinter::visit(const LiteralExpr &e) const {
	return std::to_string(e.value);
}

string AstPrinter::visit(const UnaryExpr &e) const {
	return parenthesize(e.op.str, *e.right);
}

// #define ExprBuilder(X, sep)
// 	X(Binary)sep
// 	X(Grouping)sep
// 	X(Literal)sep
// 	X(Unary)
