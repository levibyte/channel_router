#ifndef logic_h
#define logic_h

#include <SDL.h>

#include <vector>
#include <map>
#include <list>
#include <string>
#include <sstream>
//#include <ostream>
//#include <istream>
#include <iostream>
#include <limits.h>
#include <algorithm>

#include "renderer.h"
#include "graph.h"

const int CHANNEL_MAX=10;

enum ZTermOrientation { z_lower_row = CHANNEL_MAX, z_upper_row = 0  };

class ZNet;

class ZTerm
{
    public:
	    ZTerm(unsigned int col,ZTermOrientation orient,ZNet* net):m_colnum(col),m_orient(orient),m_owner_net(net) {
                //std::cout << m_
            }
	         
	    ZNet* net() { return  m_owner_net; }     
	    unsigned int col() { return m_colnum; }
	    unsigned int row() { return m_orient; }
    private:
	    unsigned int m_colnum;
	    ZTermOrientation m_orient;
	    ZNet* m_owner_net;
	    //unsigned int x;
	    //unsigned int y;
};


class ZNet 
{
    
    public:
	  //get_terms()
	  ZNet (const std::string& str ):m_name(str) { 
	      m_farest_term = 0;
	      m_closest_term = 0;
	      is_first_term = true;
	    
	  }
	  
	  ZTerm* add_term(unsigned int c, ZTermOrientation o) {
	      //std::cout << "Nearest:" << m_closest_term<< " Farest:" << m_farest_term << std::endl;
	      ZTerm* t = new ZTerm(c,o,this);
	      terms.push_back(t);
	      
	      if ( is_first_term ) {
		m_closest_term = t;
		m_farest_term = t;
		is_first_term = false;
	        return t;
	      }
	      
	      if ( m_closest_term && c < m_closest_term->col()) m_closest_term = t;
	      if ( m_farest_term && c > m_farest_term->col()) m_farest_term = t;
	      
	      return t;
	  }
	  
	  std::list<ZTerm*> get_terms() {
              return terms;
          }
          
          ZTerm* get_closest_term() {
	    assert(m_closest_term);
	    return m_closest_term;
	    
	  }
	  
	  ZTerm* get_farest_term() { 
	    assert(m_farest_term);
	    return m_farest_term;
	  }
	  
	  std::string get_name() { return m_name; }
    private:
	  //std::list<ZTerm*> terms;
	 
          ZTerm* m_farest_term;
	  ZTerm* m_closest_term;
	  bool is_first_term;
	  
	  std::list<ZTerm*> terms;
	  std::string m_name;
};





class ZChannelRouter
{

  public:
	
        ZChannelRouter():maxtracks(CHANNEL_MAX),m_max_used_track(0),m_is_done(false) {

        }
            
	void add_term_to_net(ZNet* N,unsigned int col, ZTermOrientation o) {
	    ZTerm* t = N->add_term(col,o);
	    
	    if ( o == z_lower_row )
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
              
	      if ( a1 == a2 || b1 == b2 ) return true;
	      if ( a1 > a2 ) return ( b2 > a1 && b2 < b1 );
	      if ( b1 > b2 ) return ( a2 > a1 && a2 < b2 );
              return false;
	}
	
	bool check_can_be_assigned_on_track(ZNet* n, unsigned t) {
	  std::vector<ZNet*> routed_nets = m_track2nets[t];
	  std::vector<ZNet*>::iterator i;
	  std::cout << "  Checking " << n->get_name() << " on track " << t << std::endl;
          
           if( !routed_nets.size() ) return true;
           
          //std::cout << "  on track are " << routed_nets.size() << " net" << std::endl;
          for(i=routed_nets.begin();i!=routed_nets.end();++i ) {
            //std::cout << "                "<< (*i)->get_name() << std::endl;
	    if ( has_confliscts(n,*i) ) return false;
	  }
	  
	  
	  return true;
	}
	
	bool try_to_assign_net_to_track_if_not_try_other(ZNet* net, unsigned int& track ) {
	    
	   
            //track++;
	    //return true;
	     std::cout << " Trying " << net->get_name() << " on track " << track << std::endl;
             
	    if ( track > CHANNEL_MAX ) return false;
	    
	    if ( check_can_be_assigned_on_track(net,track) ) {
	      assign_net_to_track(net,track); 
	      return true;
	    }
	    	  
	    track++;	  
            
	    return try_to_assign_net_to_track_if_not_try_other(net,track);
	}
	
	
	void route() {
	    m_is_done = false;
           
	    std::cout << " Routin net num: " << m_nets.size() << std::endl;

            create_vcg();
	    if ( m_graph->isCyclic() ) { 
		std::cout << "cycles found , can't route" << std::endl;
		return;
	    }

	   
	    
	    

	    //bool 
            unsigned int c_track=1;
	    while( ! m_is_done ) {
		std::list<ZNet*> nets = m_graph->get_top_nets();
		std::list<ZNet*>::iterator i;
		std::cout << "********************************** have " << nets.size() << " TOP nets to route" << std::endl;
		if( ! nets.size() ) break;
		
		for(i=nets.begin();i!=nets.end();++i,c_track=1)
		    try_to_assign_net_to_track_if_not_try_other(*i, c_track);
	   }
         
         
	    m_is_done = true;
	}
	
	
	
	
	void create_vcg() {
	    m_graph = new Graph(m_nets.size());
	    std::vector<ZTerm*>::iterator i;
	    int j=0;
	    for(i=bottom_terms.begin();i!=bottom_terms.end();++i,++j)
	      if ( j<top_terms.size() && !(*i)->net()->get_name().empty() && !top_terms[j]->net()->get_name().empty() ) 
		m_graph->addEdge((*i)->net(),top_terms[j]->net()), std::cout <<(*i)->net()->get_name()  << " " << top_terms[j]->net()->get_name() << " " << std::endl; 
  ;
	}


        unsigned get_maxtracks() {
          //std::cout << maxtracks << std::endl;
	  return maxtracks;
        }
        
        bool is_done() {
	    return m_is_done;
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
	
	//FIXME returning private data!
	std::list<ZNet*> get_nets() {
          return m_nets;
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
	 void assign_net_to_track(ZNet* N, unsigned int t) { 
            std::cout << "     Assigned: " << N->get_name() << " -> " << t << std::endl;
            std::cout << std::endl;
            
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
	 unsigned int m_max_used_track;
	 //class Graph<ZNet*>;
	 Graph* m_graph;
	 
         std::list<ZNet*> m_nets;
	 std::vector<ZTerm*> top_terms;
	 std::vector<ZTerm*> bottom_terms;
	 
};


class ZNetlister;

class ZRow1 { 
  public:  
        ZRow1(ZNetlister* n ):m_netlister(n) {
          
        }
        
  ZNetlister* m_netlister;
  
};


class ZRow2 { 
  public:  
        ZRow2(ZNetlister* n ):m_netlister(n) {
          
        }
        
  ZNetlister* m_netlister;
  
};

class ZRow3 { 
  public:  
        ZRow3(ZNetlister* n ):m_netlister(n) {
          
        }
        
  ZNetlister* m_netlister;
  
};


class ZRow4 { 
  public:  
        ZRow4(ZNetlister* n ):m_netlister(n) {
          
        }
        
  ZNetlister* m_netlister;
  
};


class ZNetlister 
{
  
    public:
    
           
    public:
            ZNetlister(ZChannelRouter* r):m_router(r) {
              top_num = 0;
              buttom_num = 0;
            }
            
            
           
            
            ZRow1 top_row() {
                //add_term(stream.str(),top_num,z_lower_row); 
                //top_num++;
                return ZRow1(this);
            }
            
            ZRow3 buttom_row()  {
                //add_term(name,buttom_num,z_upper_row); 
                //buttom_num++;
                return ZRow3(this);
            }
            
            void add_term1(const std::string& name) {
                // if (name.empty()) return;
                
                ZNet* net = m_router->get_or_create_net(name);
                m_router->add_term_to_net(net,top_num,z_upper_row); 
                std::cout << " Adding buttom term " << name << " " << top_num << std::endl;
                top_num++;
            }

           void add_term2(const std::string& name) {
                 //if (name.empty()) return;
                 ZNet* net = m_router->get_or_create_net(name);
                 m_router->add_term_to_net(net,buttom_num,z_lower_row);
                
                  std::cout << " Adding lower term " << name << " " << buttom_num << std::endl;
                   buttom_num++;

                 // std::cout << " Adding lower term " << name << std::endl;
            }
            
            /*
            void add_term(std::string name, unsigned int i, ZTermOrientation o) {
               if (name.empty()) return;
               ZNet* net = m_router->get_or_create_net(name);
               m_router->add_term_to_net(net,i,o); 
            }*/
            

            
          
    private:
         unsigned int top_num;
         unsigned int buttom_num;
         
         
         ZChannelRouter* m_router;
         
         
};



///ESOVAAARAAAA :D

ZRow1 operator << (ZRow2 D, std::string value) {
    //D.m_netlister->buttom_num++;
    D.m_netlister->add_term1(value);
    return ZRow1(D.m_netlister);
}


ZRow2 operator << (ZRow1 D, std::string value) {
    //D.m_netlister->buttom_num++;
    D.m_netlister->add_term1(value);
    return ZRow2(D.m_netlister);
}


ZRow3 operator << (ZRow4 D, std::string value) {
    //D.m_netlister->top_num++;
    D.m_netlister->add_term2(value);
    return ZRow3(D.m_netlister);
}


ZRow4 operator << (ZRow3 D, std::string value) {
    // D.m_netlister->top_num++;
    D.m_netlister->add_term2(value);
    return ZRow4(D.m_netlister);
}


class ZInterLayer {
  
  public:
	 ZInterLayer (const ZRender& re, ZChannelRouter* ro):m_renderer(re),m_router(ro) {
	   
	 }
	 
	 
	 void draw() {
              draw_tracks();
	      draw_nets(); 
              m_renderer.refresh();
	  }
	  

	  void draw_tracks() {
		  m_renderer.set_drawing_color(48,2,2);
		  for(unsigned int i=m_router->get_maxtracks();i>0;i--) 
		    m_renderer.draw_line(0,row_to_y(i),400,row_to_y(i));
	  }
	  
	  void draw_nets() {
	      std::list<ZNet*> nets = m_router->get_nets();
              std::list<ZNet*>::iterator i; 
              for(i=nets.begin();i!=nets.end();++i) 
                if(!(*i)->get_name().empty()) 
		  draw_individual_net(*i);  
	  }
	  
	  void draw_individual_net(ZNet* net) {
              pick_color_for_net(net);
	      
              draw_terms(net->get_terms());
              if ( m_router->is_done() ) 
                draw_routed_segments(net);
          }
	 
          void draw_terms(const std::list<ZTerm*>& terms) {
              std::list<ZTerm*>::const_iterator j; 
              for(j=terms.begin();j!=terms.end();++j) {
                draw_term(*j);
                draw_extensions(*j);
              }
          }

          
          void draw_term(ZTerm* t) {
            //std::cout << " drawrect " <<  t->col() << "---" <<  t->row() << std::endl;
            //m_renderer.draw_rect(20*t->col()+20,100*t->row()+20, 10, 10);
              m_renderer.draw_square(col_to_x(t->col()),row_to_y(!t->row()?0:m_router
              ->get_maxtracks()+1),10);
            //m_renderer->draw_text(t.row,)
          }

	  void draw_routed_segments(ZNet* n) {
	      if (m_router->get_net_track(n))
              draw_net_trunk_on_track(n);
	      //draw_extensions(n);
              //draw_intersection_points();
	  }
	  
	  void draw_net_trunk_on_track(ZNet* n) {
	      unsigned int t = m_router->get_net_track(n);
	      unsigned int c1 = n->get_closest_term()->col();
	      unsigned int c2 = n->get_farest_term()->col();

	      m_renderer.set_drawing_color(fixme[n].r,fixme[n].g,fixme[n].b);
	      //m_renderer.draw_line(0,10*i+50,20+300,10*i+50);

	      //std::cout << "Net:" << n->get_name() << " track:" << t << "  (" << c1 << "->" << c2 << ")" << std::endl;
	      //20*t->col()+20,100*t->row()+20
	      
	      m_renderer.draw_line(col_to_x(c1),row_to_y(t),col_to_x(c2),row_to_y(t));
             
	  }
	  
	  
	  void draw_extensions(ZTerm* t) {
            if ( m_router->is_done() && ! t->net()->get_name().empty() && m_router->get_net_track(t->net()) )  {
	      int tmp1 = row_to_y(!t->row()?0:m_router->get_maxtracks()+1);
	      int tmp2 = row_to_y(m_router->get_net_track(t->net()));
              m_renderer.draw_line(col_to_x(t->col()),tmp1,col_to_x(t->col()),tmp2);
              m_renderer.draw_point(col_to_x(t->col()),tmp2,2);
            }
         }

          void change_colors() {
            std::map<ZNet*,ZColor>::iterator i;
            
            for(i=fixme.begin();i!=fixme.end();++i)
              fixme[(*i).first] = get_rand_color();
          }

  private:
	 unsigned int col_to_x(unsigned int col) {
            return 20*col+20;
	 }
	 
	 unsigned int row_to_y(unsigned int o) {
	   return 20*o+20;
	 }
	 

        struct ZColor { int r; int g; int b; };

	 ZColor get_rand_color() {
            ZColor z_color;
            z_color.r = rand()%255;
            z_color.g = rand()%255;
            z_color.b = rand()%255;     

            return z_color;
         }

	  int hash_get(const std::string& str){
		int hash = 5381;
		for (int i = 0; i < str.size(); i++) {
		  hash = ((hash << 5) + hash) + (int)str[i]; /* hash * 33 + c */
		}
		
	  return hash;
	  }

	  ZColor hash_color(const std::string& str) {
            ZColor z_color;
	    int hash = hash_get(str);
	    z_color.r = (hash & 0xFF0000) >> 16;// = rand()%255;
            z_color.g = (hash & 0xFF0000) >> 8 ;// = rand()%255;
            z_color.b = (hash & 0xFF0000); // = rand()%255;     
	    std::cout << str << "->" << hash << " " << z_color.r << std::endl;
            
	    //std::cout << z_color.r << std::endl;
	    //std::cout << z_color.g << std::endl;
	    //std::cout << z_color.b << std::endl;
	    //std::cout << std::endl;
	    
            return z_color;
         }

	 void pick_color_for_net(ZNet* n) {
            
            if ( fixme.find(n) == fixme.end() ) {
	      ZColor z_color = get_rand_color();//hash_color(n->get_name()); //
	      fixme[n]=z_color;
	    }
            
            m_renderer.set_drawing_color(fixme[n].r,fixme[n].g,fixme[n].b);
        }

  private:
      ZRender m_renderer;
      ZChannelRouter* m_router;

      std::map<ZNet*, ZColor> fixme;

};




#endif