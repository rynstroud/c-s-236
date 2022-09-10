//
// Created by Camryn Stroud on 9/13/20.
//

#ifndef LEXER_PROJECT_IDAUTOMATON_H
#define LEXER_PROJECT_IDAUTOMATON_H
#include <cctype>
#include "Automaton.h"
#include "Token.h"

class IDAutomaton : public Automaton
{
private:

public:
    IDAutomaton(Token::TokenType type) : Automaton(type) {}
    ~IDAutomaton() {}

    virtual size_t read(const std::string& input)
    {
        if (!isalpha(input.at(0))) return 0;
        else
        {
            Automaton::newLines = 0;
            size_t read = 1;
            while ((isalpha(input[read])) || (isdigit(input[read])))
            {
                ++read;

                if (read == input.size() - 1) break;

                if ((!isalpha(input[read])) && (!isdigit(input[read]))) return read;
                continue;
            }
            return read;
        }
    }
    Token* createToken(std::string input, size_t lineNumber)
    { return Automaton::createToken(input, lineNumber); }

    virtual size_t newLinesRead() const { return newLines; }

};
#endif //LEXER_PROJECT_IDAUTOMATON_H
