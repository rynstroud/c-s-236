//
// Created by Camryn Stroud on 9/13/20.
//

#ifndef LEXER_PROJECT_ILLEGAL_STRING_H
#define LEXER_PROJECT_ILLEGAL_STRING_H
#include "Automaton.h"
#include "Comment.h"

class IllegalString : public Automaton // For block comments "#|''|#" that fail to close
{
public:
    IllegalString() : Automaton(Token::UNDEFINED) {} // This is an automaton, but it doesn't create a unique token type
    ~IllegalString() {}

    virtual size_t read(const std::string& input)
    {
        size_t read = 0;
        Automaton::newLines = 0;

        // Can't be an illegal string if it doesn't start out legal
        if (input.at(read) != '\'')  return 0;
        if (read >= input.size() - 1) return read + 1;

        ++read;
        if  (read >= input.size() - 1) return read + 1; // prevents string out of bounds error
        if (input[read] == '\n') ++Automaton::newLines;

        //Handles cases like ''
        if (input[read] == '\'') return 0;

        // Parsing through the information contained in the comment until it gets to '\'' or end of input
        while (read < input.size() - 1)
        {

            // Checking the next symbol
            ++read;
            if (read == input.size()) return read + 1; // Again checking for string out of bounds
            if (input[read] == '\n') ++Automaton::newLines;

            // The next symbol is '\'', which signifies part of the end of a string, so it's not illegal
            if (input[read] == '\'')
            {
                ++read;
                if (read == input.size()) return read + 1; // Again checking for string out of bounds
                if (input[read] == '\'') continue;
                else return 0;
            }

            continue;
        }

        return read + 1;
    }

    Token* createToken(std::string input, size_t lineNumber) { return Automaton::createToken(input, lineNumber); }

    virtual size_t newLinesRead() const { return newLines; }

};

#endif //LEXER_PROJECT_COMMENT_H
