//
// Created by Camryn Stroud on 9/13/20.
//

#ifndef LEXER_PROJECT_STRINGAUTOMATON_H
#define LEXER_PROJECT_STRINGAUTOMATON_H
#include "Automaton.h"
#include "Token.h"

class StringAutomaton : public Automaton
{
private:

public:
public:
    StringAutomaton(Token::TokenType type) : Automaton(type) {}
    ~StringAutomaton() {}

    virtual size_t read(const std::string& input)
    {
        size_t read = 0;
        Automaton::newLines = 0;

        // Checking to see if it even starts out as a string
        if (input.at(read) != '\'') return 0;
        ++read;

        if (read >= input.size() - 1)
        {
            if (input[read] == '\'') return read;
            return 0; // prevents string out of bounds error, undefined will be found by illegal string file
        }
        if (input[read] == '\n') ++Automaton::newLines;

        while (read < input.size() - 1)
        {
            if (input[read] == '\'')
            {
                if (read >= input.size() - 1) return 0; // prevents string out of bounds error, undefined will be found by illegal string file
                ++read;
                if (read >= input.size() - 1) return read; // prevents string out of bounds error, undefined will be found by illegal string file
                else if (input[read] == '\n') return read;
                else if (isspace(input[read])) return read;
                else if (input[read] == '\'')
                {
                    ++read;
                    if (input[read] == '\n') ++Automaton::newLines;
                    continue;
                }
                else return read;
            }

            ++read;
            if (input[read] == '\n') ++Automaton::newLines;


            if (read >= input.size() - 1) return 0; // prevents string out of bounds error, undefined will be found by illegal string file

            //continue;
        }
        return read + 1;
    }
    Token* createToken(std::string input, size_t lineNumber)
            { return Automaton::createToken(input, lineNumber); }

    virtual size_t newLinesRead() const { return newLines; }

};

#endif //LEXER_PROJECT_STRINGAUTOMATON_H
