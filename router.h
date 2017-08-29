#ifndef router_h
#define router_h



#include <vector>
#include <map>
#include <set>
#include <list>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "graph.h"
#include "connectivity.h"


/*
struct comparator
{
    inline bool operator() (ZNet* n1, ZNet* n2)
    {
        //std::cout << " Compare " << n1->get_name() <<  ":" << n1->get_closest_term()->col() << " &&& " << n2->get_name() << ":" << n2->get_closest_term()->col() << std::endl;
        return (n1->get_closest_term()->col() < n2->get_closest_term()->col());
    }
};
/**/


class ZChannelRouter
{

  
  public:
        ZChannelRouter():maxtracks(CHANNEL_MAX),m_routed_num(0),m_max_used_track(0),m_is_done(false) {

        }
        
        void add_net_to_route(ZNet* n) {
          assert(n);
          assert(!n->get_name().empty());
          get_or_create_net(n->get_name());
          //assert same net
          
          
          //m_nets.push_back(n);
          //add_term_to_net(net,buttom_num,z_lower_row);
        }
        
        void add_term_to_net(ZNet* N,unsigned int col, ZTermOrientation o) {
            ZTerm* t = N->add_term(col,o);
            store_term(t);
        }
    
        void route() {
            m_is_done = false;
            route_impl();
            //assert( m_nets.size() == m_routed_num );
            std::cout << " Missed " << m_nets.size() - m_routed_num << " nets" << std::endl;
            m_is_done = true;
          
        }
  
      
        unsigned get_maxtracks() {
          //std::cout << maxtracks << std::endl;
          return maxtracks;
        }
        
        bool is_done() {
            return m_is_done;
        }
        

        
        //FIXME returning private data!
        std::list<ZNet*> get_nets() {
          return m_nets;
        }
        
      unsigned int get_net_track(ZNet* N) {
          //assert(N);
          //assert(N->get_name().empty());
          //assert(!m_net2track[N]);
         if (N->get_name().empty()) return 9999999;
          //if ( ! m_net2track[N] ) return 9999999;
          
          //std::cout << "Request: " << N->get_name() << "--->" << m_net2track[N]  << std::endl;
          return m_net2track[N];
        }
  
  
        ZNet* get_or_create_net(const std::string& name) {
              //std::cout << "get or create "  << name << std::endl;
              std::list<ZNet*>::iterator i;
              
              bool found = false;
              for(i=m_nets.begin();i!=m_nets.end();++i){
                if( (*i)->get_name() == name ) {
                  found = true;
                  break;
                }
              }
                
              if(!found)  { 
                //std::cout << this << "not found: " << std::endl;
                ZNet* net = new ZNet(name); 
                m_nets.push_back(net);
                  //std::cout << m_nets.size() << std::endl;
                return net;
              }
              
              return *i;
        }
        
  private:
	
        void store_net_terms (ZNet* n) {
          std::list<ZTerm*> terms = n->get_terms();
          for ( std::list<ZTerm*>::iterator i = terms.begin(); i != terms.end(); ++i )
            store_term(*i);
          
        }
            
	
	
	 void store_term(ZTerm* t) {
           if ( t->row() == z_lower_row )
                top_terms.push_back(t);
            else
                bottom_terms.push_back(t);
        }
        
	bool has_confliscts(ZNet* n1, ZNet* n2) {
	      unsigned int a1 = n1->get_closest_term()->col();
	      unsigned int b1 = n1->get_farest_term()->col();
	      
	      unsigned int a2 = n2->get_closest_term()->col();
	      unsigned int b2 = n2->get_farest_term()->col();
	      
              //std::cout << "    is a confilict?  " << n1->get_name() << " and " << n2->get_name() << std::endl;
              //std::cout << "                     "  << a1 << "," << b1 << " <=> "<< a2 << "," << b2 << std::endl;
              
	      //return true;
	      
	      if ( a1 == a2 || b1 == b2 ) return true;
	      if ( a1 == b2 || b1 == a2 ) return true;
	      if ( b1 == a1 || b2 == a2 ) return true;
	      
	      if ( a1 > a2 ) return ( b2 >= a1 ); //&& b2 < b1 );
	      if ( a2 > a1 ) return ( a2 <= b1 ); 
	      //if ( b1 > b2 ) return ( a2 > a1 && a2 < b2 );
              return true;
	}
	
	bool check_can_be_assigned_on_track(ZNet* n, unsigned t) {
	  std::vector<ZNet*> routed_nets = m_track2nets[t];
	  std::vector<ZNet*>::iterator i;
	  //std::cout << "  Checking " << n->get_name() << " on track " << t << std::endl;
          
           if( !routed_nets.size() ) return true;
           
          //std::cout << "  on track are " << routed_nets.size() << " net" << std::endl;
          for(i=routed_nets.begin();i!=routed_nets.end();++i ) {
            //std::cout << "                "<< (*i)->get_name() << std::endl;
	    if ( has_confliscts(n,*i) ) return false;
	  }
	  
	  
	  return true;
	}
	
	
	bool try_to_assign(ZNet* net, unsigned track) {
          if ( check_can_be_assigned_on_track(net,track) ) {
              assign_net_to_track(net,track); 
              return true;
            }
            
         return false; 
        }
        
	bool try_to_assign_net_to_track_if_not_try_other(ZNet* net, unsigned int& track ) {
	     //track++;
	    //return true;
	     //std::cout << " Trying " << net->get_name() << " on track " << track << std::endl;
             
	    if ( track > CHANNEL_MAX ) return false;
	    
	    if ( check_can_be_assigned_on_track(net,track) ) {
	      assign_net_to_track(net,track); 
	      return true;
	    }
	    	  
	    track++;	  
            
	    return try_to_assign_net_to_track_if_not_try_other(net,track);
	}
	
	

	
	void route_impl() {
	   
	    std::cout << " Nets to route: " << m_nets.size() << std::endl;

            create_vcg();
	    if ( m_graph->isCyclic() ) { 
		std::cout << "cycles found , can't route" << std::endl;
		return;
	    }

	    //bool 
            unsigned int c_track=1;
	    while( ! m_is_done ) {
		std::vector<ZNet*> nets = m_graph->get_top_nets();
                std::cout << "********************************** have " << nets.size() << " TOP nets to route" << std::endl;
                // not for list :/ 
                //std::sort(nets.begin(), nets.end(),comparator());
                //nets.sort(comparator());
		std::vector<ZNet*>::iterator i;
		
		if( ! nets.size() ) break;
		
		for(i=nets.begin();i!=nets.end();++i) {
		    if ( try_to_assign(*i, c_track) ) 
                      m_graph->decrease_refnums(*i);
                    else
                      m_graph->return_back(*i);
                }        
                       

		c_track++;
	   }
	}
	
	
	
	
	void create_vcg() {
	    m_graph = new Graph(m_nets.size());
	    std::vector<ZTerm*>::iterator i;
	    int j=0;
	    for(i=bottom_terms.begin();i!=bottom_terms.end();++i,++j)
	      if ( j<top_terms.size() && !(*i)->net()->get_name().empty() && !top_terms[j]->net()->get_name().empty() ) 
		m_graph->addEdge((*i)->net(),top_terms[j]->net());
		
		//std::cout <<(*i)->net()->get_name()  << " " << top_terms[j]->net()->get_name() << " " << std::endl; 
  
	}


        
  private:
	 void assign_net_to_track(ZNet* N, unsigned int t) { 
            std::cout << "     !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Assigned: " << N->get_name() << " -> " << t << std::endl;
            //std::cout << std::endl;
            
	   m_routed_num++; 
           m_net2track[N] = t; 
	   m_track2nets[t].push_back(N);
	    
	   if ( t > m_max_used_track ) {
	      m_max_used_track = t;
	      maxtracks = t;
	    }
         }

  private:
    
         bool m_is_done;
         std::map<ZNet*,unsigned int> m_net2track;
	 std::map<unsigned int,std::vector<ZNet*> > m_track2nets;
	 
	 unsigned int maxtracks;
	 unsigned int m_routed_num;
	 unsigned int m_max_used_track;
	 //class Graph<ZNet*>;
	 Graph* m_graph;
	
         
         
         
         std::list<ZNet*> m_nets;
	 std::vector<ZTerm*> top_terms;
	 std::vector<ZTerm*> bottom_terms;
	 
};









#endif