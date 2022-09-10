//
// Created by Camryn Stroud on 9/24/20.
//

#ifndef LAB2_PARSER_PREDICATE_H
#define LAB2_PARSER_PREDICATE_H
#include "Parameter.h"
#include <string>
#include <sstream>
#include <vector>


class Predicate
{
private:
    std::string id;
    std::string type;
    std::vector<Parameter*> paramList;
public:
    Predicate(const std::string i, const std::string t) : id(i), type(t) {}
    Predicate(Token t) : id(t.getItem()), type(t.getType()) {}
    ~Predicate() {}
    std::string getType() const { return type; }
    std::string getID() const { return id; }
    void addParam(Parameter*& p) { paramList.push_back(p); }
    std::vector<Parameter*> getParameters() const { return paramList; }
    std::string toString() const
    {
        try {
            std::ostringstream os;
            if (paramList.empty()) throw "Uhh something went wrong in Predicate.h";
            if (type == "SCHEME") {
                os << id << '(';
                for (size_t i = 0; i < paramList.size() - 1; i++) os << paramList.at(i)->toString() << ',';
                os << paramList.at(paramList.size() - 1)->toString() << ")";
            } else if (type == "FACT") {
                os << id << '(';
                for (size_t i = 0; i < paramList.size() - 1; i++) os << paramList.at(i)->toString() << ',';
                os << paramList.at(paramList.size() - 1)->toString() << ").";
            } else if (type == "QUERY") {
                os << id << '(';
                for (size_t i = 0; i < paramList.size() - 1; i++) os << paramList.at(i)->toString() << ',';
                os << paramList.at(paramList.size() - 1)->toString() << ")?";
            } else {
                if (paramList.size() == 0) throw "Hey guys predicate iis brpken\n";
                os << id << '(';
                for (size_t i = 0; i < paramList.size() - 1; i++) os << paramList.at(i)->toString() << ',';
                os << paramList.at(paramList.size() - 1)->toString() << ")";
            }
            return os.str();
        } catch (std::string errStr) { throw std::move(errStr); }
    }
    friend std::ostream& operator<<(std::ostream& os, const Predicate& myClass) { return os << myClass.toString(); } // Overrides the extraction operator
};


#endif //LAB2_PARSER_PREDICATE_H
