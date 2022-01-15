#include "expr.hpp"

#define acceptBuilder(tp) \
	std::string tp##Expr::accept(const ExprVisitor &visitor) const { \
		return visitor.visit(*this); \
	}

ExprBuilder(acceptBuilder, )
