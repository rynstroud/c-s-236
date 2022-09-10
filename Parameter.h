//Ryn Stroud on 9/24/20.
//

#ifndef LAB2_PARSER_PARAMETER_H
#define LAB2_PARSER_PARAMETER_H

#include <string>
#include <sstream>
#include "Token.h"


class Parameter
{
protected:
    std::string id;
    bool constant;
public:
    Parameter(std::string i) : id(i), constant(false) {}
    Parameter(Token t) : id(t.getItem()), constant(false) {}
    Parameter() {}
    virtual ~Parameter() {}
    void setID(std::string s) { id = s; }
    std::string getID() { return id; }
    void setConst(bool q) { constant = q; }
    bool isConstant() { return constant; }
    virtual std::string toString() const { return id; }
};



#endif //LAB2_PARSER_PARAMETER_H
