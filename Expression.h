//
// Ryn Stroud on 10/7/20.
//

#ifndef LAB2_PARSER_EXPRESSION_H
#define LAB2_PARSER_EXPRESSION_H
#include <sstream>
#include "Parameter.h"
#include "Token.h"


class Expression : public Parameter
{
private:
    std::string type;
    Parameter* p1;
    const char oper;
    Parameter* p2;
public:
    Expression(std::string i) : Parameter(i), type("EXPRESSION"), oper(0) {}
    Expression(Token t) : Parameter(t.getItem()), type(t.getType()), oper(0) {}
    Expression(Parameter* param1, char op, Parameter* param2) : type("EXPRESSION"), p1(param1),
                oper(op), p2(param2)
                {
                    Parameter::setID(p1->toString() + oper + p2->toString());
                }
    ~Expression() {}
    std::string getType() const { return type; }
    std::string toString() const
    {
        std::ostringstream os;
        os << "(" << p1->toString() << oper << p2->toString() << ")";
        return os.str();
    }
    friend std::ostream& operator<<(std::ostream& os, const Expression& myClass) { return os << myClass.toString(); } // Overrides the extraction operator

};
#endif //LAB2_PARSER_EXPRESSION_H
