#include <iostream>
#include <list>
#include <limits.h>

#include "graph.h"
#include "connectivity.h"

Graph::Graph(int V)
{
    this->V = V;
    adj = new list<int>[V];
    n = -1;
 
    fixme.resize(V);
    for(int i=0;i<V;i++) 
      fixme[i] = 0;  
}
 
int Graph::create_or_get_net2int_mapping(ZNet* net) {
  if (fixme_net2int.find(net) == fixme_net2int.end() )
    ++n,fixme_net2int[net] = n,int2net_fixme[n]=net;
  

  return fixme_net2int[net];
}



std::vector<ZNet*> Graph::get_top_nets() {
  //std::cout << "!!!!!!!!!!!!!!!!!!!!!!! get top nets " << std::endl;
  std::vector<ZNet*> nets;
    
  
    for(int i=0;i<V;i++) {
      //std::cout << int2net_fixme[i]->get_name() << "---> " << fixme[i] << std::endl;
      if ( fixme[i] == 0 ) {
        fixme[i]=-1;
        nets.push_back(int2net_fixme[i]);
      }
    }
    
    return nets;        
}


void Graph::decrease_refnums(ZNet* n) {

    int i = create_or_get_net2int_mapping(n);
    for(list<int>::iterator j = adj[i].begin(); j!=adj[i].end(); ++j) {
      //std::cout << n->get_name() << " decr Refnum of " << int2net_fixme[*j]->get_name() << std::endl;
      fixme[*j]--;
    }
   
}


void Graph::return_back(ZNet* n) {
  
     std::cout<<"     !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Rollback: " << n->get_name() << std::endl;
    fixme[fixme_net2int[n]]=0;
}
  
void Graph::addEdge(ZNet* v, ZNet* w)
{
    
     
     int V = create_or_get_net2int_mapping(v);
     int W = create_or_get_net2int_mapping(w);
     
    //std::cout << V << " " << W << " " << std::endl; 
    adj[V].push_back(W); // Add w to vs list.
    fixme[W]++;
}
 
// This function is a variation of DFSUytil() in http://www.geeksforgeeks.org/archives/18212
bool Graph::isCyclicUtil(int v, bool visited[], bool *recStack)
{
    if(visited[v] == false)
    {
        // Mark the current node as visited and part of recursion stack
        visited[v] = true;
        recStack[v] = true;
 
        // Recur for all the vertices adjacent to this vertex
        list<int>::iterator i;
        for(i = adj[v].begin(); i != adj[v].end(); ++i)
        {
            if ( !visited[*i] && isCyclicUtil(*i, visited, recStack) )
                return true;
            else if (recStack[*i])
                return true;
        }
 
    }
    recStack[v] = false;  // remove the vertex from recursion stack
    return false;
}
 
// Returns true if the graph contains a cycle, else false.
// This function is a variation of DFS() in http://www.geeksforgeeks.org/archives/18212
bool Graph::isCyclic()
{
    // Mark all the vertices as not visited and not part of recursion
    // stack
    bool *visited = new bool[V];
    bool *recStack = new bool[V];
    for(int i = 0; i < V; i++)
    {
        visited[i] = false;
        recStack[i] = false;
    }
 
    // Call the recursive helper function to detect cycle in different
    // DFS trees
    for(int i = 0; i < V; i++)
        if (isCyclicUtil(i, visited, recStack))
            return true;
 
    return false;
}
