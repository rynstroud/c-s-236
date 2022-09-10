//
// Created by Ryn Stroud on 9/8/20.
//


#ifndef LEXER_LEXER_H
#define LEXER_LEXER_H
#include "Token.h"
#include "MatcherAutomaton.h"
#include "Undefined.h"
#include "StringAutomaton.h"
#include "IDAutomaton.h"
#include "EOFAutomaton.h"
#include "Comment.h"
#include "IllegalComment.h"
#include "IllegalString.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cctype>

class Lexer
{
private:
    size_t maxRead;
    size_t maxNewLines;


    // Vector pointing to all automata created
    std::vector<Automaton*> machine = { new MatcherAutomaton(",", Token::COMMA),
                                               new MatcherAutomaton(".", Token::PERIOD),
                                               new MatcherAutomaton("?", Token::Q_MARK),
                                               new MatcherAutomaton("(", Token::LEFT_PAREN),
                                               new MatcherAutomaton(")", Token::RIGHT_PAREN),
                                               new MatcherAutomaton(":", Token::COLON),
                                               new MatcherAutomaton(":-", Token::COLON_DASH),
                                               new MatcherAutomaton("*", Token::MULTIPLY),
                                               new MatcherAutomaton("+", Token::ADD),
                                               new MatcherAutomaton("Schemes", Token::SCHEMES),
                                               new MatcherAutomaton("Facts", Token::FACTS),
                                               new MatcherAutomaton("Rules", Token::RULES),
                                               new MatcherAutomaton("Queries", Token::QUERIES),
                                               new IllegalComment(),
                                               new IllegalString(),
                                               new IDAutomaton(Token::ID),
                                               new StringAutomaton(Token::STRING),
                                               new CommentAutomaton(Token::COMMENT),
                                               new EOFAutomaton(Token::END_OF_FILE),
                                               new Undefined()};

    Automaton* maxMachine;
    std::vector<Token> tokens; // Vector of all tokens found in data
public:
    // Parses through all provided data
    Lexer() : maxRead(0), maxNewLines(0), maxMachine(NULL) {}

    // Parameterized constructor to parse through data starting at a certain point
    Lexer(size_t r) : maxRead(r), maxNewLines(0), maxMachine(machine.at(0)) {}
    void run(std::string input);
    std::vector<Token> getTokens() { return tokens; }

    // Deletes all pointers in vectors
    ~Lexer()
    {
        for (size_t i = 0; i < machine.size(); i++)
        {
            delete machine.at(i);
            machine.erase(machine.begin());
        }
        /*for (size_t i = 0; i < tokens.size(); i++)
        {
            delete tokens.at(i);
            tokens.erase(tokens.begin());
        }*/
    }

    // Returns string with all tokens in vector and total number of tokens
    std::string toString() const
    {
        std::ostringstream os;
        for (size_t i = 0; i < tokens.size(); i++) os << tokens.at(i);
        os << "Total Tokens = " << tokens.size();
        return os.str();
    }

    // Overrides the extraction operator
    friend std::ostream& operator<<(std::ostream& os, const Lexer& myClass) { return os << myClass.toString(); }

};

// Parsing through all input to check for tokens
void Lexer::run(std::string input)
{
    const size_t LOWER_PRECEDENCE = 4; // Number of automata that need to be evaluated only if all the other automata fail
    const size_t LOWEST_PRECEDENCE = 2; // Number of automata that need to be evaluated if everything fails (EOF, Undefined)
    size_t lineNumber = 1;
    while (input.size() > 0) // Parsing through the entire input
    {
        // Removing any newlines and WS
        input = input.substr(input.size() - machine.at(machine.size() - 1)->read(input), input.size());
        lineNumber += machine.at(machine.size() - 1)->newLinesRead();

        if (input.size() < 1) break;

        maxNewLines = 0;
        maxRead = 0;
        maxMachine = machine.at(0);

        // String, Comment, EOF, and Undefined will be evaluated separately b/c lower precedence
        for (size_t i = 0; i < machine.size() - LOWER_PRECEDENCE; i++)
        {
            size_t inputRead = machine.at(i)->read(input);
            if (inputRead > maxRead)
            {
                maxRead = inputRead;
                maxMachine = machine.at(i);
                maxNewLines = machine.at(i)->newLinesRead();
            }
        }

        if (maxRead == 0)
        {
            // Evaluates String and Comment
            for (size_t i = machine.size() - LOWER_PRECEDENCE; i < machine.size() - LOWEST_PRECEDENCE; i++)
            {
                size_t inputRead = machine.at(i)->read(input);
                if (inputRead > maxRead)
                {
                    maxRead = inputRead;
                    maxMachine = machine.at(i);
                    maxNewLines = machine.at(i)->newLinesRead();
                }
            }
        }

        if (maxRead > 0) //second part takes out comments
        {
            if (maxMachine != machine.at(machine.size()-3))
            {
                Token newToken = *maxMachine->createToken(input.substr(0, maxRead), lineNumber);
                tokens.push_back(newToken);
            }
        }

        // No machine accepted the input, so it is invalid
        else
        {
            // Evaluates for Undefined tokens
            maxRead = 1;
            Token newToken = *machine.at(machine.size() - 1)->createToken(input.substr(0, maxRead), lineNumber);
            tokens.push_back(newToken);
        }

        // Moving to the next token
        lineNumber += maxNewLines;
        input = input.substr(maxRead, input.size() - maxRead);
    }

    // Evaluates for EOF
    // Finds EOF by checking for an empty string after all the input has been evaluated
    maxRead = machine.at(machine.size() - 2)->read(input);
    Token newToken = *machine.at(machine.size() - 2)->createToken(input.substr(0, maxRead), lineNumber);
    tokens.push_back(newToken);
}

#endif //LEXER_LEXER_H
