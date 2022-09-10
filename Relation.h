//
// Created by Camryn Stroud on 11/3/20.
//

#ifndef LAB2_PARSER_RELATION_H
#define LAB2_PARSER_RELATION_H
#include <string>
#include <set>
#include <sstream>
#include <iostream>
#include <list>
#include <utility>
#include "Header.h"
#include "Tuple.h"
#include "Database.h"


class Relation
{
private:
//    std::string united = "";
    std::string name;
    Header header;
    std::set<Tuple> tuples; //the set should be empty when declared here
public:
    Relation(std::string  n, const Header& h) : name(std::move(n)), header(h) {}
    Relation() {}
    ~Relation() {}
    void addTuple(const Tuple& t) { tuples.insert(t); }
    void setName(std::string n) { name = n; }
    std::string getName() const { return name; }
    Header getHeader() const { return header; }
    std::set<Tuple> getTuples() const { return tuples; }
    void setHeader(const Header& h) { header = h; }
    bool empty() { return tuples.empty(); }
    size_t size() { return tuples.size(); }

    Relation select1(size_t index, const std::string& value)
    {
        Relation r;
        r.setHeader(header);
        for (auto it = tuples.begin(); it != tuples.end(); ++it)
        {
            //adds tuples to the relation if the tuples are the same as the value
            if (it->getVal(index) == value) r.addTuple(*it);
        }
//        std::cout << "Relation " << name << " in select1: \n" << *this << std::endl;
        return r;
    }

    Relation select2(size_t index1, size_t index2, Relation& r1)
    {
        Relation r;
        r.setHeader(r1.getHeader());
        for (auto it = tuples.begin(); it != tuples.end(); ++it)
        {
            //adds tuples to the relation if the index of the tuple is the same as the index provided
            if (it->getVal(index1) == it->getVal(index2)) r.addTuple(*it);
        }
//        std::cout << "Relation " << name << " in select2: \n" << *this << std::endl;
        return r;
    }

    Relation project(const std::vector<int>& indices, Relation& r1)
    {
        Relation r;
        r.setHeader(r1.getHeader());

        //adds all tuples with the given indices to new relation
        for (auto i = tuples.begin(); i != tuples.end(); ++i)
        {
            Tuple t;
            for (size_t j = 0; j < indices.size(); j++)
            {
                t.addVal(i->getVal(indices.at(j)));
            }
            r.addTuple(t);
        }
//        std::cout << "Relation " << name << " in project: \n" << *this << std::endl;
        return r;
    }

    Relation rename(const Header& attributes)
    {
        //gives the relation a new name
        setHeader(attributes);
//        std::cout << "Relation " << name << " in rename: \n" << *this << std::endl;
        return *this;
    }

    bool isJoinable(const Tuple& t1, const Tuple& t2, const std::vector<std::pair<int, int>>& vals)
    {
        for (size_t i = 0; i < vals.size(); i++)
        {
            if (t1.getVal(vals.at(i).first) != t2.getVal(vals.at(i).second)) return false;
        }
        return true;
    }

    Tuple joinTuples(const Tuple& t1, const Tuple& t2, const std::vector<std::pair<int, int>>& vals, std::vector<int> u)
    {
        Tuple t;
        //populates the new tuple with all the values of t1
        for (size_t j = 0; j < t1.size(); j++) t.addVal(t1.getVal(j));

        //adds the values unique to t2 to the new tuple
        for (size_t j = 0; j < u.size(); j++) t.addVal(t2.getVal(u.at(j)));
        return t;
    }

    std::vector<std::pair<int,int>> combineHeaders(const Header& h1, const Header& h2, Relation& r, std::vector<int>& u)
    {
        Header h = h1;
        std::vector<std::pair<int,int>> pairs;
        for (size_t i = 0; i < h2.size(); i++)
        {
            std::string new_val = h2.getAttributes().at(i);
            int j = h1.hasMatch(new_val);
            if (j != -1)
            {
                std::pair<int, int> p1(j, i); //pushing the indices of h1 and h2 that match as a pair
                pairs.push_back(p1);
            }
            else
            {
                h.addAttribute(new_val);
                u.push_back(i);
            }
        }
        r.setHeader(h);
//        std::cout << "Relation " << name << " in combineHeaders: \n" << *this << std::endl;
        return pairs;
    }

    Relation join(const Relation& r1, const Relation& r2) //ALWAYS POSSIBLE
    {
        Relation r;
        r.setHeader(r1.header);
        std::vector<int> unique_r2_vals;
        std::vector<std::pair<int,int>> pairs = combineHeaders(r1.header, r2.header, r, unique_r2_vals);


        for (auto t1 = r1.tuples.begin(); t1 != r1.tuples.end(); ++t1)
        {
            for (auto t2 = r2.tuples.begin(); t2 != r2.tuples.end(); ++t2)
            {
                if (isJoinable(*t1, *t2, pairs))
                {
                    Tuple t = joinTuples(*t1, *t2, pairs, unique_r2_vals);
                    r.addTuple(t);
                }
                else continue;
            }
        }
        return r;
    }

    Relation unite(Relation& r1, std::ostringstream& os)
    {
        Relation r2;
        r2.setHeader(header);
        r2.setName(name);
        size_t i = 0;
        for (auto it = r1.tuples.begin(); it != r1.tuples.end(); ++it)
        {
            //adds tuples of r to the relation we are in if the tuples are not already there
            //should insert regardless but it is a set so won't have duplicates
            if (tuples.insert(*it).second)
            {
                os << "  ";
                for (size_t i = 0; i < it->size(); i++)
                {
                    if (i == it->getAllVals().size() - 1)
                    {
                        os << getHeader().getAttributes().at(i) << "=" << it->getAllVals().at(i) << "\n";
                    }
                    else os << getHeader().getAttributes().at(i) << "=" << it->getAllVals().at(i) << ", ";
                }
            }
            i++;
        }
        for (auto it = tuples.begin(); it != tuples.end(); ++it) r2.addTuple(*it);
        return r2;
    }

    std::string toString() const
    {
        std::ostringstream os;
        if (header.getAttributes().empty()) return "";
        for (auto it = tuples.begin(); it != tuples.end(); ++it)
        {
            os << "  ";
            for(size_t i = 0; i < header.getAttributes().size(); i++)
            {
                if (i == header.getAttributes().size() - 1) os << header.getAttributes().at(i) <<"="<< it->getVal(i) << "\n";
                else os << header.getAttributes().at(i) << "="
                << it->getVal(i) << ", ";
            }
        }
        return os.str();
    }

    // Overrides the extraction operator
    friend std::ostream& operator<<(std::ostream& os, const Relation& myClass) { return os << myClass.toString(); }

};


#endif //LAB2_PARSER_RELATION_H
