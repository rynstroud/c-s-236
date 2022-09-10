//
// Created by Camryn Stroud on 11/3/20.
//

#ifndef LAB2_PARSER_TUPLE_H
#define LAB2_PARSER_TUPLE_H
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>


class Tuple
{
private:
    std::vector<std::string> values;
    std::string name;
public:
    Tuple() {}
    Tuple(std::string n) : name(n) {}
    ~Tuple() {}

    void addVal(const std::string& t) { values.push_back(t); }
    std::string getName() { return name; }
    std::string getVal(size_t index) const
    {
        if (values.empty()) return "something went wrong and tuples were deleted";
        return values.at(index);
    }
    size_t size() const { return values.size(); }
    std::vector<std::string> getAllVals() const { return values; }

    int hasMatch(const std::string& s) const
    {
        for (size_t i = 0; i < values.size(); i++) if (values.at(i) == s) return i;
        return -1;
    }

    bool operator<(const Tuple & other) const { return (this->values < other.values); }

    std::string toString() const
    {
        std::ostringstream os;
        for (size_t i = 0; i < values.size(); i++) os << "\t" <<  values.at(i);
        return os.str();
    }

    // Overrides the extraction operator
    friend std::ostream& operator<<(std::ostream& os, const Tuple& myClass) { return os << myClass.toString(); }

};


#endif //LAB2_PARSER_TUPLE_H
