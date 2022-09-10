//
// Created by Camryn Stroud on 12/8/20.
//

#ifndef LAB2_PARSER_NODE_H
#define LAB2_PARSER_NODE_H
#include <set>


class Node
{
private:
    int id;
    std::set<int> adjacency_list;
    bool isVisited;
    bool selfDependence;
public:
    Node(int i) : id(i), isVisited(false), selfDependence(false) {}
    bool getIsVisited() const { return isVisited; }
    void visited() { isVisited = true; }
    void unvisited() { isVisited = false; }
    void setSelfDependent() { selfDependence = true; }
    bool isSelfDependent() const { return selfDependence; }
    std::set<int> getAdjList() const { return adjacency_list; }
    int getID() const { return id; }
    bool addAdj(const int x)
    {
        if (!adjacency_list.insert(x).second) return false;
        return true;
    }

    std::string toString() const
    {
        try
        {
            std::ostringstream os;
            os << "R" << id << ":";
            for (auto it = adjacency_list.begin(); it != adjacency_list.end(); it++)
            {
                os << "R" << *it << ",";
            }
            if (!adjacency_list.empty()) return os.str().substr(0, os.str().size()-1);
            return os.str();
        } catch (std::string errStr) { throw std::move(errStr); }
    }

    friend std::ostream& operator<<(std::ostream& os, const Node& myClass) { return os << myClass.toString(); } // Overrides the extraction operator


};


#endif //LAB2_PARSER_NODE_H
