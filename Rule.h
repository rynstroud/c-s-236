//
// Created by Camryn Stroud on 9/24/20.
//

#ifndef LAB2_PARSER_RULE_H
#define LAB2_PARSER_RULE_H
#include <sstream>
#include <vector>
#include "Predicate.h"


class Rule
{
private:
    Predicate head;
    std::vector<Predicate> preds;
public:
    Rule(Predicate headPredicate) : head(headPredicate) {}
    ~Rule() {}
    void addPredicate(Predicate p) { preds.push_back(p); }
    Predicate getHead() { return head; }
    std::vector<Predicate> getPredicate() { return preds; }
    std::string toString() const
    {
        try {
            std::ostringstream os;
            os << head << " :- ";
            for (size_t i = 0; i < preds.size() - 1; i++) os << preds.at(i) << ',';
            os << preds.at(preds.size() - 1) << '.';
            return os.str();
        } catch (std::string errStr) { throw std::move(errStr); }
    }
    friend std::ostream& operator<<(std::ostream& os, const Rule& myClass) { return os << myClass.toString(); } // Overrides the extraction operator

};


#endif //LAB2_PARSER_RULE_H
