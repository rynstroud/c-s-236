//
// Created by Ryn Stroud on 9/8/20.
//

#ifndef LEXER_TOKEN_H
#define LEXER_TOKEN_H
#include <string>
#include <sstream>
//#include "MatcherAutomaton.h"

class Token
{
public: //dude I put public first so I could have the private data member be a TokenType
    enum TokenType  {COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON,
        COLON_DASH, MULTIPLY, ADD, SCHEMES, FACTS, RULES, QUERIES, ID, STRING,
        COMMENT, UNDEFINED, END_OF_FILE};
    Token() : item("no item defined"), lineNumber(0), type(UNDEFINED) {}
    Token(TokenType t, std::string i, size_t l) : item(i), lineNumber(l), type(t) {}
    ~Token() {}
    std::string getType() const
    {
        const std::string typeNames[] = { "COMMA", "PERIOD", "Q_MARK", "LEFT_PAREN", "RIGHT_PAREN", "COLON",
                                          "COLON_DASH", "MULTIPLY", "ADD", "SCHEMES", "FACTS", "RULES", "QUERIES", "ID", "STRING",
                                          "COMMENT", "UNDEFINED", "EOF"};
        return typeNames[type];
    }
    std::string getItem() const { return item; }
    size_t getLineNumber() const { return lineNumber; }

    std::string toString() const
    {
        const std::string typeNames[] = { "COMMA", "PERIOD", "Q_MARK", "LEFT_PAREN", "RIGHT_PAREN", "COLON",
                                              "COLON_DASH", "MULTIPLY", "ADD", "SCHEMES", "FACTS", "RULES", "QUERIES", "ID", "STRING",
                                              "COMMENT", "UNDEFINED", "EOF"};
        std::ostringstream os;
        if (typeNames[type] == "COMMENT") return ""; //*comments are ignored by parser!*//
        os << "(" << typeNames[type] << ",\"" << item << "\"," << lineNumber << ")" << std::endl;
        return os.str();
    }
    // Overrides the extraction operator
    friend std::ostream& operator<<(std::ostream& os, const Token& myClass) { return os << myClass.toString(); }
private:
    std::string item;
    size_t lineNumber;
    TokenType type;
};
#endif //LEXER_TOKEN_H
