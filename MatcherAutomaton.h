//
// Created by Ryn Stroud on 9/8/20.
//

#ifndef LEXER_MATCHERAUTOMATON_H
#define LEXER_MATCHERAUTOMATON_H
#include <string>
#include "Automaton.h"
#include "Token.h"

class MatcherAutomaton : public Automaton
{
private:
    std::string toMatch;
public:
    MatcherAutomaton() : toMatch("NO STRING") {}
    MatcherAutomaton(const std::string& toMatch, const Token::TokenType& tokenType) : Automaton(tokenType), toMatch(toMatch) {}
    ~MatcherAutomaton() {}

    virtual size_t read(const std::string& input)
    {
        bool isMatch = true;
        size_t inputRead = 0;
        Automaton::newLines = 0;
        for (size_t i = 0; i < toMatch.size() && isMatch; i++)
        {
            if (input[i] != toMatch[i]) isMatch = false;
        }
        if (isMatch) inputRead = toMatch.size();
        //Automaton::newLines = inputRead;
        return inputRead;
    }

    size_t newLinesRead() const { return Automaton::newLinesRead(); }
    Token* createToken(std::string input, size_t lineNumber) { return Automaton::createToken(input, lineNumber); }

    std::string toString() const { return toMatch; }
    // Overrides the extraction operator
    friend std::ostream& operator<<(std::ostream& os, const MatcherAutomaton& myClass) { return os << myClass.toString(); }
};


#endif //LEXER_MATCHERAUTOMATON_H
