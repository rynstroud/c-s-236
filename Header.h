//
// Created by Camryn Stroud on 11/3/20.
//

#ifndef LAB2_PARSER_HEADER_H
#define LAB2_PARSER_HEADER_H
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>

class Header
{
private:
    std::vector<std::string> attributes;
public:
    Header() {}
    ~Header() {}
    void addAttribute(std::string a) { attributes.push_back(a); }
    std::vector<std::string> getAttributes() const { return attributes; }
    size_t size() const { return attributes.size(); }

    int hasMatch(const std::string& s) const
    {
        for (size_t i = 0; i < attributes.size(); i++) if (attributes.at(i) == s) return i;
        return -1;
    }

    std::string toString() const
    {
        if (attributes.size() == 0)
        {
            return "There is nothing here in Header??";
        }
        std::ostringstream os;
        for (size_t i = 0; i < attributes.size(); i++) os << std::setw(10) << std::fixed << attributes.at(i) << "\t";
        return os.str();

    }
    // Overrides the extraction operator
    friend std::ostream& operator<<(std::ostream& os, const Header& myClass) { return os << myClass.toString(); }
};


#endif //LAB2_PARSER_HEADER_H
