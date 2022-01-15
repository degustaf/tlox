#ifndef TLOX_SCANNER_HPP
#define TLOX_SCANNER_HPP

#include <istream>
#include <string>

enum TokenType {
	// Single-character tokens.
	TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
	TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
	TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS,
	TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,
	// One or two character tokens.
	TOKEN_BANG, TOKEN_BANG_EQUAL,
	TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
	TOKEN_GREATER, TOKEN_GREATER_EQUAL,
	TOKEN_LESS, TOKEN_LESS_EQUAL,
	// Literals.
	TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,
	// Keywords.
	TOKEN_AND, TOKEN_CLASS, TOKEN_ELSE, TOKEN_FALSE,
	TOKEN_FOR, TOKEN_FUN, TOKEN_IF, TOKEN_NIL, TOKEN_OR,
	TOKEN_PRINT, TOKEN_RETURN, TOKEN_SUPER, TOKEN_THIS,
	TOKEN_TRUE, TOKEN_VAR, TOKEN_WHILE,

	TOKEN_ERROR, TOKEN_EOF, TOKEN_NULL
};
static_assert(TOKEN_BANG_EQUAL    == TOKEN_BANG    + 1, "");
static_assert(TOKEN_EQUAL_EQUAL   == TOKEN_EQUAL   + 1, "");
static_assert(TOKEN_GREATER_EQUAL == TOKEN_GREATER + 1, "");
static_assert(TOKEN_LESS_EQUAL    == TOKEN_LESS    + 1, "");

class Scanner;

struct Token {
	TokenType type;
	std::string str;
	size_t line;

	void makeToken(TokenType type, const std::string &s, size_t line);
	void makeToken(TokenType type, char c, size_t line);
	void make2CharToken(TokenType type, char c, Scanner &s);
	void errorToken(const std::string &msg, size_t line);
	bool isNull(void) const;
};

class Scanner {
	public:
		Scanner(std::istream &src) : src(src), line(1) {
			cache.makeToken(TOKEN_NULL, "", 0);
		};
		Scanner& operator>> (Token &t);
		Token peek(void);
	private:
		bool match(char c);
		void skipWhitespace(void);
		Scanner& stringToken(Token &t);
		Scanner& numberToken(char c, Token &t);
		Scanner& identifierToken(char c, Token &t);
		friend void Token::make2CharToken(TokenType tp, char c, Scanner &s);

		std::istream &src;
		size_t line;
		Token cache;
};

#endif /* TLOX_SCANNER_HPP */
