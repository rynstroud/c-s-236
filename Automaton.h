//
// Created by Ryn Stroud on 9/8/20.
//
#ifndef LEXER_AUTOMATON_H
#define LEXER_AUTOMATON_H
#include "Token.h"
class Automaton
{
protected:
    size_t newLines;
    Token::TokenType type;
public:
    Automaton() : newLines(0), type(Token::UNDEFINED) {}
    Automaton(Token::TokenType type) : newLines(0) { this->type = type; }
    virtual ~Automaton() {}
    virtual size_t read(const std::string& input) { return 0; }
    virtual Token* createToken(std::string input, size_t lineNumber) {
        return new Token(type, input, lineNumber); }
    virtual size_t newLinesRead() const { return newLines; }
};
#endif //LEXER_AUTOMATON_H
