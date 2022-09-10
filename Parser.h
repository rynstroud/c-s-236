//
// Created by Ryn Stroud on 9/24/20.
//

#ifndef LAB2_PARSER_PARSER_H
#define LAB2_PARSER_PARSER_H
#include <string>
#include <sstream>
#include <exception>
#include "Token.h"
#include "DatalogProgram.h"
#include "PlainParameter.h"
#include "Expression.h"
#include "Predicate.h"
#include "Rule.h"


class Parser
{
private:
    size_t index;
    DatalogProgram data;
    std::vector<Token> tokens;
    int isPredicate; // adds at the beginning of expression, subtracts at end, only adds params if 0
public:
    Parser(std::vector<Token> t) : index(0), isPredicate(0) { tokens = t; }
    ~Parser() {}
    void match(const std::string& s);
    bool mini_match(const std::string& s);
    DatalogProgram* datalogProgram();
    void schemeList();
    void factList();
    void ruleList();
    void queryList();
    void scheme();
    void fact();
    void rule();
    void query();
    Predicate headPredicate();
    Predicate predicate(const std::string& t);
    void predicateList(Rule& r);
    void parameterList(Predicate& p);
    void stringList(Predicate& p);
    void idList(Predicate& p);
    Parameter* parameter(Predicate& p);
    Parameter* expression(Predicate& p);
    char operatorToken(); // I had to call it operatorToken because operator is a reserved word for overriding operators haha
    std::string toString() const;

    friend std::ostream& operator<<(std::ostream& os, const Parser& myClass) { return os << myClass.toString(); } // Overrides the extraction operator
};


void Parser::match(const std::string& s)
{
    std::string input = tokens.at(index).getType();
    for (size_t i = 0; i < input.size(); i++)
    {
        if (input.at(i) != s.at(i)) throw tokens.at(index);
    }
    ++index;
}

bool Parser::mini_match(const std::string& s)
{
    std::string new_input = tokens.at(index).getType();
    for (size_t i = 0; i < s.size(); i++)
    {
        if (s.at(i) != new_input.at(i)) return false;
    }
    return true;
}

DatalogProgram* Parser::datalogProgram()
{
    try
    {
        match("SCHEMES");
        match("COLON");
        scheme();
        schemeList();
        match("FACTS");
        match("COLON");
        factList();
        match("RULES");
        match("COLON");
        ruleList();
        match("QUERIES");
        match("COLON");
        query();
        queryList();
        match("EOF");
    } catch (Token errorTok)
    {
        //std::string errorStr = errorTok.toString().substr(0,errorTok.toString().size()-1);
        std::string errorStr = errorTok.toString();
        throw "Failure!\n  " + errorStr; //+ '\n';
    }
    return &data;
}

void Parser::schemeList()
{
    if (mini_match("ID"))
    {
        scheme();
        schemeList();
    }
    else return;
}

void Parser::factList() {
    if (mini_match("ID"))
    {
        fact();
        factList();
    }
    else return;
}

void Parser::ruleList() {
    if (mini_match("ID"))
    {
        rule();
        ruleList();
    }
    else return;
}

void Parser::queryList() {
    if (mini_match("ID"))
    {
        query();
        queryList();
    }
    else return;
}

void Parser::scheme()
{
    try {

        match("ID");
        Predicate newScheme = Predicate(tokens.at(index - 1).getItem(), "SCHEME");
        match("LEFT_PAREN");
        if (mini_match("ID")) {
            match("ID");
            Parameter* param = new Parameter(tokens.at(index-1).getItem());
            param->setConst(false);
            newScheme.addParam(param);
        } else throw "Failure!\n  " + tokens.at(index).toString();
        idList(newScheme);
        match("RIGHT_PAREN");
        data.addScheme(newScheme);
    } catch (std::string errStr) { throw std::move(errStr); }
}

void Parser::fact()
{
    match("ID");
    Predicate newFact = Predicate(tokens.at(index-1).getItem(), "FACT");
    match("LEFT_PAREN");
    match("STRING");
    Parameter* param = new Parameter(tokens.at(index-1).getItem());
    param->setConst(true);
    newFact.addParam(param);
    data.addToDomain(tokens.at(index-1).getItem());
    stringList(newFact);
    match("RIGHT_PAREN");
    match("PERIOD");
    data.addFact(newFact);
}

void Parser::rule()
{
    Predicate head = headPredicate();
    Rule newRule(head);
    match("COLON_DASH");
    Predicate p = predicate("RULE_PRED");
    newRule.addPredicate(p);
    predicateList(newRule);
    match("PERIOD");
    data.addRule(newRule);
}

void Parser::query()
{
    Predicate q = predicate("QUERY");
    match("Q_MARK");
    data.addQuery(q);
}

Predicate Parser::headPredicate()
{
    match("ID");
    Predicate newHead(tokens.at(index-1).getItem(), "HEAD");
    match("LEFT_PAREN");
    match("ID");
    Parameter* param = new Parameter(tokens.at(index-1).getItem());
    param->setConst(false);
    newHead.addParam(param);
    idList(newHead);
    match("RIGHT_PAREN");
    return newHead;
}

Predicate Parser::predicate(const std::string& t)
{
    match("ID");
    Predicate p(tokens.at(index-1).getItem(), t);
    match("LEFT_PAREN");
    parameter(p);
    parameterList(p);
    match("RIGHT_PAREN");
    return p;
}

void Parser::predicateList(Rule& r)
{
    if (mini_match("COMMA"))
    {
        match("COMMA");
        Predicate p = predicate("RULE_PRED");
        r.addPredicate(p);
        predicateList(r);
    }
    else return;
}

void Parser::parameterList(Predicate& p)
{
    if (mini_match("COMMA"))
    {
        match("COMMA");
        parameter(p);
        parameterList(p);
    }
    else return;
}

void Parser::stringList(Predicate& p)
{
    if (mini_match("COMMA"))
    {
        match("COMMA");
        match("STRING");
        Parameter* param = new Parameter(tokens.at(index-1).getItem());
        param->setConst(true);
        if (isPredicate == 0) p.addParam(param);
        if (p.getType() == "FACT") data.addToDomain(tokens.at(index-1).getItem());
        stringList(p);
    }
    else return;
}

void Parser::idList(Predicate& p)
{
    if (mini_match("COMMA"))
    {
        match("COMMA");
        match("ID");
        Parameter* param = new Parameter(tokens.at(index-1).getItem());
        param->setConst(false);
        if (isPredicate == 0) p.addParam(param);
        idList(p);
    }
    else return;
}

Parameter* Parser::parameter(Predicate& p)
{
    if (mini_match("STRING"))
    {
        match("STRING");
        Parameter* param = new Parameter(tokens.at(index-1).getItem());
        param->setConst(true);
        if (isPredicate == 0) p.addParam(param);
        return param;
    }
    else if (mini_match("ID"))
    {
        match("ID");
        Parameter* param = new Parameter(tokens.at(index-1).getItem());
        param->setConst(false);
        if (isPredicate == 0) p.addParam(param);
        return param;
    }
    else if (mini_match("LEFT_PAREN"))
    {
        Parameter* exp = expression(p);
        return exp;
    }
    else throw "Failure!\n  " + tokens.at(index).toString();
}

Parameter* Parser::expression(Predicate& p)
{
    ++isPredicate;
    match("LEFT_PAREN");
    Parameter* p1 = parameter(p);
    char oper = operatorToken();
    Parameter* p2 = parameter(p);
    Parameter* exp = new Expression(p1, oper, p2);
    if (isPredicate == 1) p.addParam(exp);
    match("RIGHT_PAREN");
    --isPredicate;
    return exp;
}


char Parser::operatorToken() // I had to call it operatorToken because operator is a reserved word for overriding operators
{
    if (mini_match("ADD"))
    {
        match("ADD");
        return '+';
    }
    else if (mini_match("MULTIPLY"))
    {
        match("MULTIPLY");
        return '*';
    }
    else throw "Failure!\n  " + tokens.at(index).toString();
}

std::string Parser::toString() const
{
    try
    {
        std::ostringstream os;
        //os <<
        os << "Success!" << std::endl;
        os << data;
        return os.str();
    } catch (std::string errorStr) { throw std::move(errorStr); }
}


#endif //LAB2_PARSER_PARSER_H
