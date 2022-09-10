//
// Created by Camryn Stroud on 9/13/20.
//

#ifndef LEXER_PROJECT_ILLEGAL_COMMENT_H
#define LEXER_PROJECT_ILLEGAL_COMMENT_H
#include "Automaton.h"
#include "Comment.h"

class IllegalComment : public Automaton // For block comments "#|''|#" that fail to close
{
public:
    IllegalComment() : Automaton(Token::UNDEFINED) {} // This is an automaton, but it doesn't create a unique token type
    ~IllegalComment() {}

    virtual size_t read(const std::string& input)
    {
        Automaton::newLines = 0;

        // Can't be an illegal comment if it doesn't start out legal
        if ((input.at(0) != '#') || (0 >= input.size() - 1)) return 0;

        size_t read = 1;
        if  (read >= input.size() - 1) return read + 1; // prevents string out of bounds error, undefined will be found by illegal string file

        // Can't be an illegal comment if it doesn't start out legal
        if (input.at(1) == '|')
        {
            if  (read >= input.size() - 1) return read + 1; // prevents string out of bounds error, undefined will be found by illegal string file
            ++read;
            if (input[read] == '\n') ++Automaton::newLines;

            if ((input[read] == '|') && (read < input.size() - 1))
            {
                ++read;
                if (read >= input.size() - 1) return read + 1;
                if (input[read] == '#') return 0;
                if (input[read] == '\n') ++Automaton::newLines;
            }

            // Parsing through the information contained in the comment until it gets to |# or end of input
            while (read < input.size() - 1)
            {

                // Checking the next symbol
                ++read;
                if (input[read] == '\n') ++Automaton::newLines;
                if (read == input.size() - 1) return read + 1; // Again checking for string out of bounds


                // If the next symbol is '|', which signifies part of the end of a comment, check the next letter and if it is not '#', keep parsing.
                // Otherwise, return 0 because this is a valid block comment
                if (input[read] == '|')
                {
                    ++read;
                    if (read == input.size() - 1) return read + 1; // Again checking for string out of bounds
                    else if (input[read] == '\n') ++Automaton::newLines;
                    else if(input.at(read) != '#') continue;
                    else return 0;
                }

                // If read is not reaching end of input or  equalling '|', then keep checking symbols
                continue;
            }
        }

        // Not an illegal comment because didn't start out legal
        else return 0;
        return read + 1;
    }

    Token* createToken(std::string input, size_t lineNumber) { return Automaton::createToken(input, lineNumber); }

    virtual size_t newLinesRead() const { return newLines; }

};

#endif //LEXER_PROJECT_COMMENT_H
