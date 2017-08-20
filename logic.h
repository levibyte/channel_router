#ifndef logic_h
#define logic_h

#include <SDL.h>

#include <vector>
#include <map>
#include <list>
#include <string>

#include "renderer.h"

enum ZTermOrientation { z_upper_row, z_lower_row };

class ZTerm
{
    public:
	    ZTerm(unsigned int col,ZTermOrientation orient):m_colnum(col),m_orient(m_orient) {
	    }
	          
    private:
	    unsigned int m_colnum;
	    ZTermOrientation m_orient;
	    
	    //unsigned int x;
	    //unsigned int y;
};


class ZNet {
    
    public:
	  //get_terms()
	  ZNet (const char* str):m_name(str) { 
	    
	  }
	  
	  void add_term(unsigned int c, ZTermOrientation o) {
	      ZTerm t(c,o);
	      terms.push_back(t);
	  }
	  
	  
    private:
	  //std::list<ZTerm*> terms;
	  
	  std::list<ZTerm> terms;
	  std::string m_name;
};



class ZChannelRouter {

  public:
	
    ZChannelRouter():maxtracks(6) {
	    //*
	    a = new ZNet("A");
	    a->add_term(0,z_upper_row);
	    a->add_term(2,z_upper_row);
	    
	    b = new ZNet("B");
	    b->add_term(1,z_upper_row);
	    b->add_term(0,z_lower_row);
	    
	    c = new ZNet("C");
	    c->add_term(3,z_upper_row);
	    c->add_term(1,z_lower_row);
	    c->add_term(2,z_lower_row);
	    /**/
	}
	
	void assign_net_to_track(ZNet* N, unsigned int n) { m_net2track[N] = n; }

	void route() {
	    assign_net_to_track(a,1);
	    assign_net_to_track(b,2);
	    assign_net_to_track(c,3);
	}
	
  
  private:
	 std::map<ZNet*,unsigned int> m_net2track;
	 unsigned int maxtracks;
	 
	 ZNet* a;
	 ZNet* b;
	 ZNet* c;
};

class ZInterLayer {
  
  public:
	 ZInterLayer (const ZRender& re, const ZChannelRouter& ro):m_renderer(re),m_router(ro) {
	   
	 }
	 
	 
	 void draw() {
	    m_renderer.draw_line(0,0,50,50);
	  }
	 
	 void draw_terms() {
	      //router->get_nets();
	      //net->get_terms();
	      //m_renderer->draw_square(t.row,t.orient,t.row+5,t.orient+5);
	 }
	 
	 void draw_routed_segments() {
	      //draw_terms();
	      //draw_trunk();
	      //draw_extensions();
	 }
	 
  private:
      ZRender m_renderer;
      ZChannelRouter m_router;
};

#endif