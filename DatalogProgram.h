//
// Created by Ryn Stroud on 9/24/20.
//

#ifndef LAB2_PARSER_DATALOGPROGRAM_H
#define LAB2_PARSER_DATALOGPROGRAM_H
#include <string>
#include <sstream>
#include <exception>
#include <vector>
#include <set>
#include "PlainParameter.h"
#include "Token.h"
#include "Rule.h"
#include "Predicate.h"


class DatalogProgram {
private:
    std::vector<Predicate> schemes;
    std::vector<Predicate> facts;
    std::vector<Predicate> queries;
    std::vector<Rule> rules;
    std::set<std::string> domain;
public:
    DatalogProgram() {}
    ~DatalogProgram()
    {
        std::vector<Parameter*> schemeParam;
        for (size_t i = 0; i < schemes.size(); i++)
        {
            schemeParam = schemes.at(i).getParameters();
            for (size_t j = 0; j < schemeParam.size(); j++) delete schemeParam.at(j);
        }
        std::vector<Parameter*> factParam;
        for (size_t i = 0; i < facts.size(); i++)
        {
            factParam = facts.at(i).getParameters();
            for (size_t j = 0; j < factParam.size(); j++) delete factParam.at(j);
        }
        std::vector<Parameter*> queryParam;
        for (size_t i = 0; i < queries.size(); i++)
        {
            queryParam = queries.at(i).getParameters();
            for (size_t j = 0; j < queryParam.size(); j++) delete queryParam.at(j);
        }
        std::vector<Predicate> rulePred;
        for (size_t i = 0; i < rules.size(); i++)
        {
            rulePred = rules.at(i).getPredicate();
            for (size_t j = 0; j < rulePred.size(); j++)
            {
                std::vector<Parameter*> ruleParam = rulePred.at(j).getParameters();
                for (size_t k = 0; k < ruleParam.size(); k++) delete ruleParam.at(k);
            }
        }
    }
    void addScheme(const Predicate& p) { schemes.push_back(p); }
    std::vector<Predicate> getSchemes() { return schemes; }
    void addFact(const Predicate& p) { facts.push_back(p); }
    std::vector<Predicate> getFacts() { return facts; }
    void addRule(const Rule& p) { rules.push_back(p); }
    std::vector<Rule> getRules() { return rules; }
    void addQuery(const Predicate& p) { queries.push_back(p); }
    std::vector<Predicate> getQueries() { return queries; }
    void addToDomain(const std::string s) { domain.insert(s); }
    std::string toString() const
    {
        try
        {
            std::ostringstream os;
            if (schemes.size() > 0) os << "Schemes(" << schemes.size() << "):\n";
            else os << "Schemes(0):\n";
            for (size_t i = 0; i < schemes.size(); i++) os << "  " << schemes.at(i) << "\n";
            if (facts.size() > 0) os << "Facts(" << facts.size() << "):\n";
            else os << "Facts(0):\n";
            for (size_t i = 0; i < facts.size(); i++) os << "  " << facts.at(i) << "\n";
            if (rules.size() > 0) os << "Rules(" << rules.size() << "):\n";
            else os << "Rules(0):\n";
            for (size_t i = 0; i < rules.size(); i++) os << "  " << rules.at(i) << "\n";
            if (queries.size() > 0) os << "Queries(" << queries.size() << "):\n";
            else os << "Queries(0):\n";
            for (size_t i = 0; i < queries.size(); i++) os << "  " << queries.at(i) << "\n";
            if (domain.size() > 0) os << "Domain(" << domain.size() << "):";
            else os << "Domain(0):";
            for (auto& v: domain) os << "\n  " << v;
            os << '\n';
            return os.str();
        } catch (std::string errStr) { throw std::move(errStr); }

    }
    friend std::ostream& operator<<(std::ostream& os, const DatalogProgram& myClass) { return os << myClass.toString(); } // Overrides the extraction operator
};

#endif //LAB2_PARSER_DATALOGPROGRAM_H