#include "parser.hpp"

using std::make_shared;
using std::shared_ptr;

static bool match(const Token &t, TokenType tp) {
	return t.type == tp;
}

template<typename... Types>
static bool match(const Token &t, TokenType tp, Types... tps) {
	if(t.type == tp)
		return true;
	return match(t, tps...);
}

shared_ptr<Expr> Parser::expression(void) {
	return equality();
}

shared_ptr<Expr> Parser::equality(void) {
	shared_ptr<Expr> expr = comparison();

	Token op = scanner.peek();
	if(match(op, TOKEN_EQUAL_EQUAL, TOKEN_BANG_EQUAL)) {
		scanner >> op;
		shared_ptr<Expr> right = comparison();
		expr = make_shared<BinaryExpr>(expr, op, right);
		op = scanner.peek();
	}

	return expr;
}

shared_ptr<Expr> Parser::comparison(void) {
	shared_ptr<Expr> expr = term();

	Token op = scanner.peek();
	while(match(op, TOKEN_GREATER, TOKEN_GREATER_EQUAL, TOKEN_LESS, TOKEN_LESS_EQUAL)) {
		scanner >> op;
		op = scanner.peek();
		shared_ptr<Expr> right = term();
		expr = make_shared<BinaryExpr>(expr, op, right);
		op = scanner.peek();
	}

	return expr;
}

shared_ptr<Expr> Parser::term(void) {
	shared_ptr<Expr> expr = factor();

	Token op = scanner.peek();
	while(match(op, TOKEN_MINUS, TOKEN_PLUS)) {
		scanner >> op;
		op = scanner.peek();
		shared_ptr<Expr> right = factor();
		expr = make_shared<BinaryExpr>(expr, op, right);
		op = scanner.peek();
	}

	return expr;
}

shared_ptr<Expr> Parser::factor(void) {
	shared_ptr<Expr> expr = unary();

	Token op = scanner.peek();
	while(match(op, TOKEN_SLASH, TOKEN_STAR)) {
		scanner >> op;
		op = scanner.peek();
		shared_ptr<Expr> right = unary();
		expr = make_shared<BinaryExpr>(expr, op, right);
		op = scanner.peek();
	}

	return expr;
}

shared_ptr<Expr> Parser::unary(void) {
	Token op = scanner.peek();
	if(match(op, TOKEN_BANG, TOKEN_MINUS)) {
		scanner >> op;
		shared_ptr<Expr> right = unary();
		return make_shared<UnaryExpr>(op, right);
	}
	return primary();
}

shared_ptr<Expr> Parser::primary(void) {
	Token tk;
	scanner >> tk;

	// if(match(tk, TOKEN_FALSE))
	// if(match(tk, TOKEN_TRUE))
	// if(match(tk, TOKEN_NIL))
	if(match(tk, TOKEN_NUMBER))
		return make_shared<LiteralExpr>(stod(tk.str));
	// if(match(tk, TOKEN_STRING))
	if(match(tk, TOKEN_LEFT_PAREN)) {
		shared_ptr<Expr> expr = expression();
		consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
		return make_shared<GroupingExpr>(expr);
	}
	throw ParserError("Invalid primary expression.");
}

Token Parser::consume(TokenType tp, const std::string &msg) {
	Token tk = scanner.peek();
	if(!match(tk, tp)) {
		scanner >> tk;
		return tk;
	}
	throw ParserError(msg);
}
