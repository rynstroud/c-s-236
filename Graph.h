//
// Created by Camryn Stroud on 12/8/20.
//

#ifndef LAB2_PARSER_GRAPH_H
#define LAB2_PARSER_GRAPH_H
#include "Node.h"
#include <map>
#include <stack>
#include <iostream>
#include <vector>

class Graph //store a separate forward and backward graph
{
private:
    std::map<int,Node> nodes;
    std::vector<int> post_order;
    std::vector<std::set<int>> SCCs;
public:
    Graph() {}

    void addNode(const Node& n)
    {
        nodes.insert(std::pair<int,Node>(n.getID(), n));
    }

    Node findNode(int i) const
    {
        return nodes.find(i)->second;
    }

    void clearVisits()
    {
        for (auto it : nodes)
        {
            //unvisits the node, erases the unvisited node and replaces it with the visited one
            Node x = it.second;
            if (x.getIsVisited())
            {
                x.unvisited();
                nodes.erase(nodes.find(x.getID()));
                nodes.emplace(x.getID(), x);
            }
        }
    }

    void dfs(Node& x, std::map<int,Node>::iterator& it)
    {
        if (x.getIsVisited()) return;
        if (x.getAdjList().empty())
        {
            post_order.push_back(x.getID());
            x.visited();
            nodes.erase(nodes.find(x.getID()));
            nodes.emplace(x.getID(), x);
            return;
        }

        //visits the node, erases the unvisited node and replaces it with the visited one
        x.visited();
        nodes.erase(nodes.find(x.getID()));
        nodes.emplace(x.getID(), x);

        int i = 0;
        for (auto it : x.getAdjList())
        {
            i++;
            //If the rule is only dependent on itself, quit the loop
            if ((x.isSelfDependent()) && (i == x.getAdjList().size() - 1)) break;

            Node y = nodes.find(it)->second;
            if (!y.getIsVisited()) dfs(y, (std::map<int, Node>::iterator &) it);

            //Dealing with nodes in which all their adjacencies have been visited
            if (i == x.getAdjList().size()) break;
        }
        it = nodes.find(x.getID());
        post_order.push_back(x.getID());
    }

    std::vector<int> findPostOrder()
    {
        clearVisits();

        for (auto it : nodes)
        {
            Node x = it.second;
            if (!x.getIsVisited()) dfs(x, (std::map<int, Node>::iterator &) it);
            if (post_order.size() == nodes.size()) return post_order;
        }
        return post_order;
    }

    std::set<int> new_dfs(Node& x, std::set<int>& scc)
    {
        if (x.getIsVisited()) return scc;
        if (x.getAdjList().empty())
        {
            scc.insert(x.getID());
            x.visited();
            nodes.erase(nodes.find(x.getID()));
            nodes.emplace(x.getID(), x);
            return scc;
        }

        //visits the node, erases the unvisited node and replaces it with the visited one
        x.visited();
        nodes.erase(nodes.find(x.getID()));
        nodes.emplace(x.getID(), x);

        int i = -1;
        if (!x.getAdjList().empty())
        {
            for (auto it : x.getAdjList())
            {
                i++;
                //If the rule is only dependent on itself, quit the loop
                if ((x.isSelfDependent()) && (i == x.getAdjList().size() - 1))
                {
                    scc.insert(x.getID());
                    return scc;
                }

                Node y = nodes.find(it)->second;
                if (!y.getIsVisited()) new_dfs(y, scc);

                //Dealing with nodes in which all their adjacencies have been visited
                if (i > x.getAdjList().size() - 2)
                {
                    scc.insert(x.getID());
                    return scc;
                }
                //if (it == x.getAdjList().end()) break;
            }
        }
        scc.insert(x.getID());
        return scc;
    }

    std::vector<std::set<int>> findSCC(std::vector<int> postorder)
    {
        clearVisits();

        for (size_t i = 0; i < postorder.size(); i++)
        {
            std::set<int> scc;
            Node x = nodes.find(postorder.at(i))->second;
            if (!x.getIsVisited())
            {
                scc.clear();
                scc = new_dfs(x, scc);
                SCCs.push_back(scc);
            }
        }
        return SCCs;
    }

    void addEdge(int from, int to)
    {
        nodes.find(from)->second.addAdj(to);
        if (from == to) nodes.find(from)->second.setSelfDependent();
    }

    std::string toString() const
    {
        try
        {
            std::ostringstream os;
            os << "Dependency Graph\n";
            for (size_t i = 0; i < nodes.size(); i++)
            {
                os << nodes.find(i)->second << std::endl;
            }
            os << "\n";
            return os.str();
        } catch (std::string errStr) { throw std::move(errStr); }
    }

    friend std::ostream& operator<<(std::ostream& os, const Graph& myClass) { return os << myClass.toString(); } // Overrides the extraction operator

};


#endif //LAB2_PARSER_GRAPH_H
