//
// Created by Camryn Stroud on 9/13/20.
//

#ifndef LEXER_PROJECT_COMMENT_H
#define LEXER_PROJECT_COMMENT_H
#include "Automaton.h"

class CommentAutomaton : public Automaton
{
private:

public:
public:
    CommentAutomaton(Token::TokenType type) : Automaton(type) {}
    ~CommentAutomaton() {}
    virtual size_t read(const std::string& input)
    {
        Automaton::newLines = 0;
        size_t read = 0;

        // If it doesn't start out with right syntax, don't bother
        if (input.at(read) != '#') return 0;
        //if (read >= input.size() - 1) return read + 1; // Handles cases of just "#" as entire input
        else
        {
            ++read;
            if  (read >= input.size() - 1) return 0; // prevents string out of bounds error, undefined will be found by illegal string file

            // Handling block comments
            if (input.at(1) == '|')
            {
                ++read;
                if ((input[read] == '|') && (read < input.size() - 1))
                {
                    ++read;
                    if (read >= input.size() - 1) return 0;
                    if (input[read] == '#') return read + 1;
                }

                // Parses through until it reaches '|',' signifying end of comment
                while (input[read] != '|')
                {
                    ++read;

                    if (read >= input.size() - 1) return 0; // prevents string out of bounds error, undefined will be found by illegal string file
                    if (input[read] == '\n') ++Automaton::newLines;

                    if ((input[read] == '|') && (read < input.size() - 1))
                    {
                        ++read;
                        if (input[read] == '\n') ++Automaton::newLines;
                        if (read >= input.size() - 1) return 0; // prevents string out of bounds error, undefined will be found by illegal string file
                        if (input.at(read) != '#') continue; // Allows for comments in "#|A|B|#" format, with A and B being strings

                        return read + 1; // |# signifies end of comment, so return
                    }

                    continue;
                }
            }

            // Handling one-line comments
            else
            {
                // Continue parsing until reaching the end of the line
                while (input[read] != '\n')
                {
                    ++read;
                    if (read >= input.size()) return 0; // prevents string out of bounds error, undefined will be found by illegal string file
                    if (input[read] == '\n') return read;
                    continue;
                }
            }
            return read;
        }
    }
    Token* createToken(std::string input, size_t lineNumber)
    { return Automaton::createToken(input, lineNumber); }

    virtual size_t newLinesRead() const { return newLines; }

};

#endif //LEXER_PROJECT_COMMENT_H
