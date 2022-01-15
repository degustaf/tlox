#include "scanner.hpp"

#include <assert.h>

using std::string;

void Token::makeToken(TokenType tp, const string &s, size_t ln) {
	type = tp;
	str = s;
	line = ln;
}

void Token::makeToken(TokenType tp, char c, size_t ln) {
	string s(1,c);
	makeToken(tp, s, ln);
}

void Token::make2CharToken(TokenType tp, char c, Scanner &s) {
	string str(1,c);
	if(s.match('=')) {
		str.push_back('=');
		tp = static_cast<TokenType>(static_cast<int>(tp) + 1);
	}
	makeToken(tp, str, s.line);
}

void Token::errorToken(const std::string &msg, size_t ln) {
	type = TOKEN_ERROR;
	str = msg;
	line = ln;
}

bool Token::isNull(void) const {
	return type == TOKEN_NULL;
}

static bool isAlpha(char c) {
	return (c >= 'a' && c <= 'z') ||
		   (c >= 'A' && c <= 'Z') ||
		   c == '_';
}

static bool isDigit(char c) {
	return c >= '0' && c <= '9';
}

TokenType checkKeyword(size_t start, size_t length, const string &rest, const string &str, TokenType tp) {
	if((str.size() == start + length) && (str.compare(start, length, rest)))
		return tp;

	return TOKEN_IDENTIFIER;
}

static TokenType identifierType(const string &str) {
	switch(str[0]) {
		case 'a': return checkKeyword(1, 2, "nd", str, TOKEN_AND);
		case 'c': return checkKeyword(1, 4, "lass", str, TOKEN_CLASS);
		case 'e': return checkKeyword(1, 3, "lse", str, TOKEN_ELSE);
		case 'f':
			if(str.size() > 1) {
				switch(str[1]) {
					case 'a': return checkKeyword(2, 3, "lse", str, TOKEN_FALSE);
					case 'o': return checkKeyword(2, 1, "r", str, TOKEN_FOR);
					case 'u': return checkKeyword(2, 1, "n", str, TOKEN_FUN);
				}
			}
			break;
		case 'i': return checkKeyword(1, 1, "f", str, TOKEN_IF);
		case 'n': return checkKeyword(1, 2, "il", str, TOKEN_NIL);
		case 'o': return checkKeyword(1, 1, "r", str, TOKEN_OR);
		case 'p': return checkKeyword(1, 4, "rint", str, TOKEN_PRINT);
		case 'r': return checkKeyword(1, 5, "eturn", str, TOKEN_RETURN);
		case 's': return checkKeyword(1, 4, "uper", str, TOKEN_SUPER);
		case 't':
			if(str.size() > 1) {
				switch(str[1]) {
					case 'h': return checkKeyword(2, 2, "is", str, TOKEN_THIS);
					case 'r': return checkKeyword(2, 2, "ue", str, TOKEN_TRUE);
				}
			}
			break;
		case 'v': return checkKeyword(1, 2, "ar", str, TOKEN_VAR);
		case 'w': return checkKeyword(1, 4, "hile", str, TOKEN_WHILE);
	}

	return TOKEN_IDENTIFIER;
}

Scanner& Scanner::operator>> (Token &t) {
	if(!cache.isNull()) {
		t = cache;
		cache.makeToken(TOKEN_NULL, "", 0);
		return *this;
	}

	if(src.eof()) {
		t.makeToken(TOKEN_EOF, "", line);
		return *this;
	}

	char c = src.get();

	if(isDigit(c))
		return numberToken(c, t);

	if(isAlpha(c))
		return identifierToken(c, t);

	switch(c) {
		case '(': t.makeToken(TOKEN_LEFT_PAREN,  '(', line); return *this;
		case ')': t.makeToken(TOKEN_RIGHT_PAREN, ')', line); return *this;
		case '{': t.makeToken(TOKEN_LEFT_BRACE,  '{', line); return *this;
		case '}': t.makeToken(TOKEN_RIGHT_BRACE, '}', line); return *this;
		case ';': t.makeToken(TOKEN_SEMICOLON,   ';', line); return *this;
		case ',': t.makeToken(TOKEN_COMMA,       ',', line); return *this;
		case '.': t.makeToken(TOKEN_DOT,         '.', line); return *this;
		case '-': t.makeToken(TOKEN_MINUS,       '-', line); return *this;
		case '+': t.makeToken(TOKEN_PLUS,        '+', line); return *this;
		case '/': t.makeToken(TOKEN_SLASH,       '/', line); return *this;
		case '*': t.makeToken(TOKEN_STAR,        '*', line); return *this;

		case '!': t.make2CharToken(TOKEN_BANG,    '!', *this); return *this;
		case '=': t.make2CharToken(TOKEN_EQUAL,   '=', *this); return *this;
		case '<': t.make2CharToken(TOKEN_LESS,    '<', *this); return *this;
		case '>': t.make2CharToken(TOKEN_GREATER, '>', *this); return *this;

		case '"': return stringToken(t);
	}

	t.errorToken("Unexpected character.", line);
	return *this;
}

Token Scanner::peek(void) {
	if(cache.isNull()) {
		*this >> cache;
	}
	return cache;
}

bool Scanner::match(char c) {
	if(src.eof())
		return false;
	if(src.peek() == c) {
		src.get();
		return true;
	}
	return false;
}

void Scanner::skipWhitespace(void) {
	while(true) {
		char c = src.peek();
		switch(c) {
			case '/':
				src.get();
				if(src.peek() == '/') {
					while(!src.eof() && src.peek() != '\n')
						src.get();
				} else {
					src.unget();
					return;
				}
				break;
			case '\n':
				line++;
			case '\r':
			case '\t':
			case ' ':
				src.get();
				break;
			default:
				return;
		}
	}
}

Scanner& Scanner::stringToken(Token &t) {
	string ret;

	char c;
	while(!src.eof() && (c = src.peek()) != '"') {
		if(c == '\n')
			line++;
		ret.push_back(c);
		src.get();
	}

	if(src.eof()) {
		t.errorToken("Unterminated string.", line);
		return *this;
	}

	c = src.get();
	assert(c == '"');

	t.makeToken(TOKEN_STRING, ret, line);
	return *this;
}

Scanner& Scanner::numberToken(char c, Token &t) {
	string ret(1, c);

	while(isDigit(src.peek()))
		ret.push_back(src.get());

	if(src.peek() == '.') {
		ret.push_back(src.get());

		while(isDigit(src.peek()))
			ret.push_back(src.get());
	}

	t.makeToken(TOKEN_NUMBER, ret, line);
	return *this;
}

Scanner& Scanner::identifierToken(char c, Token &t) {
	string ret(1,c);

	c = src.peek();
	while(isAlpha(c) || isDigit(c)) {
		ret.push_back(src.get());
		c = src.peek();
	}

	t.makeToken(identifierType(ret), ret, line);
	return *this;
}
