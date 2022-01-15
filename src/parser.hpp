#ifndef TLOX_PARSER_HPP
#define TLOX_PARSER_HPP

#include "expr.hpp"
#include "scanner.hpp"

#include <stdexcept>

class ParserError : public std::runtime_error {
	public:
		ParserError(const std::string &msg) : std::runtime_error(msg) {};
};

class Parser {
	public:
		Parser(Scanner &scanner) : scanner(scanner) {};
	private:
		std::shared_ptr<Expr> expression(void);
		std::shared_ptr<Expr> equality(void);
		std::shared_ptr<Expr> comparison(void);
		std::shared_ptr<Expr> term(void);
		std::shared_ptr<Expr> factor(void);
		std::shared_ptr<Expr> unary(void);
		std::shared_ptr<Expr> primary(void);

		Token consume(TokenType tp, const std::string &msg);

		Scanner &scanner;
};

#endif /* TLOX_PARSER_HPP */
