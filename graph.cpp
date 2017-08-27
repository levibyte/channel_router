#include <iostream>
#include <list>
#include <limits.h>

#include "graph.h"
//#include "logic.h"

Graph::Graph(int V)
{
    this->V = V-1;
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


std::list<ZNet*> Graph::get_top_nets() {
    std::list<ZNet*> nets;
    for(int i=0;i<V;i++) {
      
      if (! fixme[i]) {
	nets.push_back(int2net_fixme[i]);
	fixme[i]=-1;
      }
	
      /*
      std::cout << i << "   " << (adj[i]).front() << " connections: " << (adj[i]).size() << std::endl;
      std::cout << " --> " << int2net_fixme[i] << std::endl;
      nets.push_back(int2net_fixme[i]);
      */
      
      /*if( (adj[i]).size() != 0 ) 
	nets.push_back(int2net_fixme[*(adj[i].begin())]);
      else
	if( i+1<=V) nets.push_back(int2net_fixme[i+1]);
      */
      
    }
    
    return nets;        
}

//void Graph::addEdge(int v, int w)

/*
void Graph::dump() {
   for(int i=0;i<V;i++) {
      std::cout << i << int2net_fixme[i]->get_name() << std::endl;
        
}*/

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
