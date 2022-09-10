//
// Created by Camryn Stroud on 11/3/20.
//

#ifndef LAB2_PARSER_DATABASE_H
#define LAB2_PARSER_DATABASE_H
#include <map>
#include <string>
#include <iomanip>
#include "Relation.h"


class Database
{
private:
    std::map<std::string, Relation> info;
public:
    Database(){}
    ~Database() {}
    void addRelation(const std::string& name, const Relation& relation) { info.insert(std::pair<std::string, Relation>(name, relation)); }
    Relation getRelation(const std::string& name) { return info.find(name)->second; }

    Relation modifyRelation(const std::string& name, const Relation& newRelation) //adding a new relation to an already existing one
    {
//        std::map<std::string, Relation>::iterator it = info.find(name);
//        if (it != info.end()) it->second = newRelation;
        info.at(name) = newRelation;
        return newRelation;
    }


    size_t size() { return info.size(); }

    std::string toString() const
    {
        std::ostringstream os;
        os << "\nThe database contains : \n";
        for (auto itr = info.begin(); itr != info.end(); ++itr)
        {
            os << "\t" << itr->first << "\n" << std::setw(13) << std::fixed << itr->second;
        }
        os << std::endl;
        return os.str();
    }

    // Overrides the extraction operator
    friend std::ostream& operator<<(std::ostream& os, const Database& myClass) { return os << myClass.toString(); }
};


#endif //LAB2_PARSER_DATABASE_H
