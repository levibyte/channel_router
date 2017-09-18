#ifndef graph_h
#define graph_h

#include <iostream>
#include <list>
#include <vector>
#include <limits.h>
#include <map> 

using namespace std;

class ZNet;

class Graph
{
    int n;
    int V;    // No. of vertices
    list<int> *adj;    // Pointer to an array containing adjacency lists
    
    vector<int> fixme; 
    bool isCyclicUtil(int v, bool visited[], bool *rs);  // used by isCyclic()
    map<ZNet*,int> fixme_net2int;
    map<int,ZNet*> int2net_fixme;
    
    
public:
  
    void decrease_refnums(ZNet* n);
    void return_back(ZNet* n);
  //void dump();
    int create_or_get_net2int_mapping(ZNet* n);
    std::vector<ZNet*> get_top_nets();

    Graph(int V);   // Constructor
    void addEdge(ZNet* v, ZNet* w);   // to add an edge to graph
    bool isCyclic();    // returns true if there is a cycle in this graph
};

#endif