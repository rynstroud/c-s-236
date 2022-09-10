//
// Ryn Stroud on 10/7/20.
//

#ifndef LAB2_PARSER_PLAINPARAMETER_H
#define LAB2_PARSER_PLAINPARAMETER_H
#include "Parameter.h"
#include "Token.h"

class PlainParameter : public Parameter
{
private:
public:
    PlainParameter(std::string i) : Parameter(i) {}
    PlainParameter(Token t) : Parameter(t.getItem()) {}
    ~PlainParameter() {}
    std::string toString() const { return id; }
    friend std::ostream& operator<<(std::ostream& os, const PlainParameter& myClass) { return os << myClass.toString(); } // Overrides the extraction operator

};
#endif //LAB2_PARSER_PLAINPARAMETER_H
