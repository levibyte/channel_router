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

const int CHANNEL_MAX=6;

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
	
        ZChannelRouter():maxtracks(CHANNEL_MAX),m_is_done(false) {

        }
            
	void add_term_to_net(ZNet* N,unsigned int col, ZTermOrientation o) {
	    ZTerm* t = N->add_term(col,o);
	    
	    if ( o == z_lower_row )
		top_terms.push_back(t);
	    else
		bottom_terms.push_back(t);
	}
	

	void route() {
	    m_is_done = false;
             std::cout << " Routin net num: " << m_nets.size() << std::endl;

            create_vcg();
	    if ( m_graph->isCyclic() ) { 
		std::cout << "cycles found , can't route" << std::endl;
		return;
	    }
	    
	    std::list<ZNet*> nets = m_graph->get_top_nets();
            std::list<ZNet*>::iterator i;
            int j=1;
            for(i=nets.begin();i!=nets.end();i++,j++)
              if(*i) assign_net_to_track(*i,j);
            
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
          return maxtracks;
        }
        
        bool is_done() {
	    return m_is_done;
	}
	
	unsigned int get_net_track(ZNet* N) {
          if (N->get_name().empty()) return 9999999;
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
            std::cout << " Assigned: " << N->get_name() << " -> " << t << std::endl;
           m_net2track[N] = t; 
         }

  private:
    
         bool m_is_done;
         std::map<ZNet*,unsigned int> m_net2track;
	 unsigned int maxtracks;
	 
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
		  m_renderer.set_drawing_color(255,0,0);
		  for(unsigned int i=m_router->get_maxtracks()-1;i>0;i--) 
		    m_renderer.draw_line(0,row_to_y(i),400,row_to_y(i));
	  }
	  
	  void draw_nets() {
	      std::list<ZNet*> nets = m_router->get_nets();
              std::list<ZNet*>::iterator i; 
              for(i=nets.begin();i!=nets.end();++i) 
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
              m_renderer.draw_square(col_to_x(t->col()),row_to_y(t->row()),10);
            //m_renderer->draw_text(t.row,)
          }

	  void draw_routed_segments(ZNet* n) {
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

	      //std::cout << "Net:" << n->get_name() << " track:" << t << " " << c1 << " " << c2 << std::endl;
	      //20*t->col()+20,100*t->row()+20
	      
	      m_renderer.draw_line(col_to_x(c1),row_to_y(t),col_to_x(c2),row_to_y(t));
             
	  }
	  
	  
	  void draw_extensions(ZTerm* t) {
            if ( m_router->is_done() && ! t->net()->get_name().empty() )  {
              m_renderer.draw_line(col_to_x(t->col()),row_to_y(t->row()),col_to_x(t->col()),row_to_y(m_router->get_net_track(t->net())));
              m_renderer.draw_point(col_to_x(t->col()),row_to_y(m_router->get_net_track(t->net())),4);
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
         
	 void pick_color_for_net(ZNet* n) {
            
            ZColor z_color = get_rand_color();
            if ( fixme.find(n) == fixme.end() ) 
            fixme[n]=z_color;
            
            m_renderer.set_drawing_color(fixme[n].r,fixme[n].g,fixme[n].b);
        }

  private:
      ZRender m_renderer;
      ZChannelRouter* m_router;

      std::map<ZNet*, ZColor> fixme;

};




#endif