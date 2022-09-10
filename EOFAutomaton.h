//
// Created by Camryn Stroud on 9/13/20.
//

#ifndef LEXER_PROJECT_EOFAUTOMATON_H
#define LEXER_PROJECT_EOFAUTOMATON_H
#include <cctype>
#include "Automaton.h"
#include "Token.h"

class EOFAutomaton : public Automaton {
private:

public:
    EOFAutomaton(Token::TokenType type) : Automaton(type) {}
    ~EOFAutomaton() {}

    virtual size_t read(const std::string &input)
    {
        if (input.size() == 0)  return 1;
        else return 0;
    }

    Token *createToken(std::string input, size_t lineNumber) { return Automaton::createToken(input, lineNumber); }

    virtual size_t newLinesRead() const { return newLines; }
};

#endif //LEXER_PROJECT_EOFAUTOMATON_H
