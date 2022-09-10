//
// Created by Camryn Stroud on 11/3/20.
//

#ifndef LAB2_PARSER_INTERPRETER_H
#define LAB2_PARSER_INTERPRETER_H
#include "DatalogProgram.h"
#include "Database.h"
#include "Graph.h"
#include "Node.h"
#include <algorithm>
#include <sstream>


class Interpreter
{
private:
    Database database;
    DatalogProgram* datalog;
public:
    Interpreter(DatalogProgram* l) : datalog(l) {}
    ~Interpreter() {}

    void run()
    {

        //Adding all scheme relations to database
         for (size_t i = 0; i < datalog->getSchemes().size(); i++)
         {
             std::string name = datalog->getSchemes().at(i).getID();
             Header h;
             for (size_t j = 0; j < datalog->getSchemes().at(i).getParameters().size(); j++)
             {
                 // This accesses the individual parameters within a scheme and adds them to the header object
                 h.addAttribute(datalog->getSchemes().at(i).getParameters().at(j)->getID());
             }
             Relation newRelation(name, h);
             database.addRelation(name, newRelation);
         }

        //Adding all fact relations to database
        for (size_t i = 0; i < datalog->getFacts().size(); i++)
        {
            std::string name = datalog->getFacts().at(i).getID();
            Tuple t(name);
            for (size_t j = 0; j < datalog->getFacts().at(i).getParameters().size(); j++)
            {
                // This accesses the individual parameters within a fact and adds them to the tuple object
                t.addVal(datalog->getFacts().at(i).getParameters().at(j)->getID());
            }
            Relation r = database.getRelation(name);
            r.addTuple(t); //adding a tuple to the relation
            database.modifyRelation(name, r); //replacing the old relation with the new relation that has the tuple
        }

        for (size_t i = 0; i < datalog->getQueries().size(); i++)
        {
            std::string name = datalog->getQueries().at(i).getID();
            Relation r = database.getRelation(name); //adding query relations to the database
        }

        for (size_t i = 0; i < datalog->getRules().size(); i++)
        {
            std::string name = datalog->getRules().at(i).getHead().getID();
        }
    }

    Relation evaluatePredicate(const Predicate& p)
    {
        std::vector<std::string> old_ids;
        std::vector<int> indices;
        Relation r = database.getRelation(p.getID());

        for (size_t i = 0; i < p.getParameters().size(); i++ )
        {
            //perform select with an index and a name
            if (p.getParameters().at(i)->isConstant())
            {
                r = r.select1(i, p.getParameters().at(i)->toString());
            }

            //add the ids in the predicate to the vector so we can add them as attributes at end of function
            else if ((std::find(old_ids.begin(), old_ids.end(), p.getParameters().at(i)->toString())) == old_ids.end())
            {
                old_ids.push_back(p.getParameters().at(i)->toString());
                indices.push_back(i);
            }

            //perform select with two indices
            else
            {
                size_t index = 0;
                for (size_t j = 0; j < old_ids.size(); j++)
                {
                    if (p.getParameters().at(i)->toString() == old_ids.at(j)) index = indices.at(j);
                }

                r = r.select2(i, index, r);
            }
        }

        r = r.project(indices, r); //project all the indices of the attributes collected
        //if (old_ids.empty()) return r;
        Header h;
        for (size_t i = 0; i < old_ids.size(); i++) h.addAttribute(old_ids.at(i));
        r.rename(h); //rename the function to the new header with the attributes collected
        return r;
    }

    std::string evaluateAllQueries()
    {
//        std::cout << "what snap is showing: \n" << database.getRelation("snap") << std::endl;
        std::ostringstream os;
        os << "\nQuery Evaluation\n";
        for (size_t i = 0; i < datalog->getQueries().size(); i++)
        {
            Predicate pred = datalog->getQueries().at(i);
            os << pred;

            //evaluates all the predicates in the loop and saves the evaluation to an os
            Relation r = evaluatePredicate(pred);

            if (r.empty()) //the query has no results
            {
                os << " No\n";
                continue;
            }
            else os << " Yes(" << r.size() << ")\n"; //outputs the number of results then the results themselves
            os << r;
        }
        return os.str();
    }

    std::string evaluateAllRules()
    {
        std::ostringstream os;
        size_t passes = 0;
        size_t rule_number = 0;

        //Makes the graphs for the depth first search
        Graph forward_graph;
        Graph reverse_graph;
        createGraphs(forward_graph, reverse_graph, os);

        os << "Rule Evaluation\n";

        //Finds the post_order of the reverse graph
        std::vector<int> post_order = findPostOrder(reverse_graph);

        std::vector<std::set<int>> sccs = findSCCs(forward_graph, reversePO(post_order));

        for (size_t i = 0; i < sccs.size(); i++) //Gets the rule
        {
            size_t t_added = 0;
            passes = 0;
            size_t scc_num = 0;
            do
            {
                t_added = 0;
                for (auto it = sccs.at(i).begin(); it != sccs.at(i).end(); it++)
                {
                    size_t j = *it;
                    rule_number = *it;

                    if (scc_num == 0)
                    {
                        os << "SCC: " << sccNames(sccs.at(i)) << std::endl;
                    }

                    //Gets og amount of tuples in rule
                    size_t r_size = getTuplesSize(j);

                    //adds the rule to the string
                    os << datalog->getRules().at(j) << "\n";

                    //Evaluates the rule
                    Relation r = evalRule(os, j);

                    //Compares amount of tuples after evaluation to before
                    if (r_size < r.getTuples().size()) ++t_added;
                    ++scc_num;

                    if (isTrivial(sccs.at(i).size(),rule_number,forward_graph)) break;
                }
                ++passes;
                if (isTrivial(sccs.at(i).size(),rule_number,forward_graph)) break;
            } while (t_added > 0);
            os << passes << " passes: " << sccNames(sccs.at(i)) << std::endl;
        }
        return os.str();
    }

    std::string sccNames(std::set<int>& scc)
    {
        std::stringstream temp;
        for (auto iter = scc.begin(); iter != scc.end(); iter++)
        {
            temp << "R" << *iter << ",";
        }
        return temp.str().substr(0,temp.str().size()-1);
    }

    bool isTrivial(const int r, const int i, const Graph& graph) const
    {
        return ((!graph.findNode(i).isSelfDependent()) && (r == 1));
    }

    std::vector<int> reversePO(std::vector<int>& PO)
    {
        std::vector<int> post_order;
        for (size_t i = 0; i < PO.size(); i++) post_order.insert(post_order.begin(), PO.at(i));
        return post_order;
    }

    size_t getTuplesSize(int i)
    {
        //shows if the amount of tuples is changing each pass through rules
        size_t r_size = 0;

        if (!database.getRelation(datalog->getRules().at(i).getHead().getID()).empty())
        {
            r_size = database.getRelation(datalog->getRules().at(i).getHead().getID()).getTuples().size();
        }
        return r_size;
    }

    Relation evalRule(std::ostringstream& os, int i)
    {
        //sets the head object to the rule's head
        Predicate head = datalog->getRules().at(i).getHead();
        std::vector<Relation> intermediate_r = vectorRules(i);

        Relation r;

        r = natural_join(intermediate_r, r);
        r = project(r, head, i);
        r = rename(head, r);
        r = unite(r, head, os);

        return r;
    }

    Relation unite(Relation& r, Predicate& head, std::ostringstream& os)
    {
        //union with the relation that matches the name of the head predicate
        r = database.getRelation(head.getID()).unite(r, os);
        database.modifyRelation(head.getID(), r);
        return r;
    }

    Relation rename(Predicate& head, Relation& r)
    {
        //rename the relation to the new header with the attributes collected from the head of the rule
        Header h;
        for (size_t j = 0; j < head.getParameters().size(); j++) h.addAttribute(head.getParameters().at(j)->getID());
        r = r.rename(h);

        return r;
    }

    Relation project(Relation& r, Predicate& head, int i)
    {
        //create the vector of indices of columns to be projected from the head predicate
        std::vector<int> indices;
        size_t head_size = head.getParameters().size();

        //loops so that the columns can reorder based on the order they appear in the head
        for (size_t k = 0; k < head_size; k++)
        {
            for (size_t j = 0; j < r.getHeader().size(); j++)
            {
                if (datalog->getRules().at(i).getHead().getParameters().at(k)->toString()
                    == r.getHeader().getAttributes().at(j))
                {
                    indices.push_back(j);
                    break;
                }
            }
        }

        //project the columns that are in the head predicate
        r = r.project(indices, r);
        return r;
    }

    Relation natural_join(std::vector<Relation>& intermediate_r, Relation& r)
    {
        //natural join the intermediate_r list
        if (intermediate_r.size() > 0)
        {
            //Sets the first intermediate relation to r
            r = intermediate_r.at(0);

            //joins r with the next intermediate relation, then sets r to that value
            // and repeats until there are no more values
            for (size_t j = 1; j < intermediate_r.size(); j++) r = r.join(r, intermediate_r.at(j));
        }
        return r;
    }

    std::vector<Relation> vectorRules(int i)
    {
        std::vector<Relation> intermediate_r;

        //Gets the predicates of the rule
        for (size_t j = 0; j < datalog->getRules().at(i).getPredicate().size(); j++)
        {
            Predicate pred = datalog->getRules().at(i).getPredicate().at(j);
            Relation r = evaluatePredicate(pred);
            intermediate_r.push_back(r);
        }

        return intermediate_r;
    }

    std::vector<int> findPostOrder(Graph& graph)
    {
        std::vector<int> post_order = graph.findPostOrder();
        return post_order;
    }

    std::vector<std::set<int>> findSCCs(Graph& graph, std::vector<int> post_order)
    {
        return graph.findSCC(post_order);
    }

    void createGraphs(Graph& forward_graph, Graph& reverse_graph, std::ostringstream& os)
    {
        for (size_t i = 0; i < datalog->getRules().size(); i++)
        {
            Node newNode(i);
            forward_graph.addNode(newNode);
            reverse_graph.addNode(newNode);
        }
        for (size_t i = 0; i < datalog->getRules().size(); i++) //Gets the head rule
        {
            for (size_t j = 0; j < datalog->getRules().size(); j++) //Gets the predicate rules
                for (size_t k = 0; k < datalog->getRules().at(j).getPredicate().size(); k++) //Gets the predicates
                {
                    if (datalog->getRules().at(i).getHead().getID()
                    == datalog->getRules().at(j).getPredicate().at(k).getID())
                    {
                        forward_graph.addEdge(j,i);
                        reverse_graph.addEdge(i,j);
                    }
                }
        }
        os << forward_graph;
    }

    std::string toString() const
    {
        std::ostringstream os;
        os << *datalog <<  database;
        return os.str();
    }

    // Overrides the extraction operator
    friend std::ostream& operator<<(std::ostream& os, const Interpreter& myClass) { return os << myClass.toString(); }
};


#endif //LAB2_PARSER_INTERPRETER_H
