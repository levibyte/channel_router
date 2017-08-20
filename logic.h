#ifndef logic_h
#define logic_h

#include <SDL.h>

#include <vector>
#include <map>
#include <list>
#include <string>

#include "renderer.h"

enum ZTermOrientation { z_lower_row = 0, z_upper_row = 1  };

class ZTerm
{
    public:
	    ZTerm(unsigned int col,ZTermOrientation orient):m_colnum(col),m_orient(orient) {
                //std::cout << m_
            }
	         
	         
	    unsigned int col() { return m_colnum; }
	    unsigned int row() { return m_orient; }
    private:
	    unsigned int m_colnum;
	    ZTermOrientation m_orient;
	    
	    //unsigned int x;
	    //unsigned int y;
};


class ZNet 
{
    
    public:
	  //get_terms()
	  ZNet (const char* str):m_name(str) { 
	    
	  }
	  
	  void add_term(unsigned int c, ZTermOrientation o) {
	      ZTerm* t = new ZTerm(c,o);
	      terms.push_back(t);
	  }
	  
	  std::list<ZTerm*> get_terms() {
              return terms;
          }
	  
	  std::string get_name() { return m_name; }
    private:
	  //std::list<ZTerm*> terms;
	  
	  std::list<ZTerm*> terms;
	  std::string m_name;
};



class ZChannelRouter
{

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
	
	//FIXME returning private data!
	std::list<ZNet*> get_nets() {
            std::list<ZNet*> z;
            z.push_back(a);
            z.push_back(b);
            z.push_back(c);
            
          return z;
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
              std::list<ZNet*> nets = m_router.get_nets();
              std::list<ZNet*>::iterator i; 
              for(i=nets.begin();i!=nets.end();++i) 
                draw_individual_net(*i);  
              
              m_renderer.refresh();
	  }
	  
	  void draw_individual_net( ZNet* net) {
              m_renderer.pick_color_from_name(net->get_name());
              draw_terms(net->get_terms());
              //if ( m_router.is_done() ) 
                //draw_net(net);
          }
	 
          void draw_terms(const std::list<ZTerm*>& terms) {
              std::list<ZTerm*>::const_iterator j; 
              for(j=terms.begin();j!=terms.end();++j) 
                draw_term(*j);
          }

          
          void draw_term(ZTerm* t) {
            //std::cout << " drawrect " <<  t->col() << "---" <<  t->row() << std::endl;
            m_renderer.draw_rect(20*t->col()+20,100*t->row()+20, 10, 10);
            //m_renderer->draw_text(t.row,)
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