#ifndef logic_h
#define logic_h

#include <SDL.h>

#include <vector>
#include <map>
#include <list>
#include <string>

#include "renderer.h"

enum ZTermOrientation { z_lower_row = 1, z_upper_row = 0  };

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
	      m_farest_term = 0;
	      m_closest_term = 0;
	      is_first_term = true;
	    
	  }
	  
	  ZTerm* add_term(unsigned int c, ZTermOrientation o) {
	      std::cout << "Nearest:" << m_closest_term<< " Farest:" << m_farest_term << std::endl;
	      ZTerm* t = new ZTerm(c,o);
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
	
    ZChannelRouter():maxtracks(6) {
	    //*
	    a = new ZNet("A");
	    add_term_to_net(a,0,z_upper_row);
	    add_term_to_net(a,2,z_upper_row);
	    
	    b = new ZNet("B");
	    add_term_to_net(b,1,z_upper_row);
	    add_term_to_net(b,0,z_lower_row);
	    
	    c = new ZNet("C");
	    add_term_to_net(c,3,z_upper_row);
	    add_term_to_net(c,1,z_lower_row);
	    add_term_to_net(c,2,z_lower_row);
	    /**/
	}
	
	void add_term_to_net(ZNet* N,unsigned int col, ZTermOrientation o) {
	    ZTerm* t = N->add_term(col,o);
	    
	    if ( o == z_lower_row )
		top_terms.push_back(t);
	    else
		bottom_terms.push_back(t);
	}
	
	void assign_net_to_track(ZNet* N, unsigned int t) { m_net2track[N] = t; }

	void route() {
	    assign_net_to_track(a,1);
	    assign_net_to_track(b,2);
	    assign_net_to_track(c,3);
	}

	bool is_done() {
	    return true;
	}
	
	unsigned int get_net_track(ZNet* N) {
	  return m_net2track[N];
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
	 
	 std::vector<ZTerm*> top_terms;
	 std::vector<ZTerm*> bottom_terms;
	 
};





class ZInterLayer {
  
  public:
	 ZInterLayer (const ZRender& re, const ZChannelRouter& ro):m_renderer(re),m_router(ro) {
	   
	 }
	 
	 
	 void draw() {
              draw_tracks();
	      draw_nets(); 
              m_renderer.refresh();
	  }
	  

	  void draw_tracks() {
	          //router.get_max_tracks_num;
		  m_renderer.set_drawing_color(255,0,0);
		  for(unsigned int i=5;i>0;i--) 
		    m_renderer.draw_line(0,10*i+50,20+300,10*i+50);
	  }
	  
	  void draw_nets() {
	      std::list<ZNet*> nets = m_router.get_nets();
              std::list<ZNet*>::iterator i; 
              for(i=nets.begin();i!=nets.end();++i) 
                draw_individual_net(*i);  
	  }
	  
	  void draw_individual_net(ZNet* net) {
              pick_color_for_net(net);
              draw_terms(net->get_terms());
              if ( m_router.is_done() ) 
                draw_routed_segments(net);
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

	  void draw_routed_segments(ZNet* n) {
	      draw_net_trunk_on_track(n);
	      //draw_extensions();
	  }
	  
	  void draw_net_trunk_on_track(ZNet* n) {
	      unsigned int t = m_router.get_net_track(n);
	      unsigned int c1 = n->get_closest_term()->col();
	      unsigned int c2 = n->get_farest_term()->col();

	      m_renderer.set_drawing_color(fixme[n].r,fixme[n].g,fixme[n].b);
	      //m_renderer.draw_line(0,10*i+50,20+300,10*i+50);

	      std::cout << "Net:" << n->get_name() << " track:" << t << " " << c1 << " " << c2 << std::endl;
	      //20*t->col()+20,100*t->row()+20
	      
	      m_renderer.draw_line(20*c1+20,10*t+50,20*c2+20,10*t+50);
	  }

  private:
	 unsigned int col_to_x(unsigned int col) {
	   
	 }
	 
	 unsigned int orient_to_y(ZTermOrientation o) {
	   
	 }
	 
	 void pick_color_for_net(ZNet* n) {
            Color z_color;
            z_color.r = rand()%255;
            z_color.g = rand()%255;
            z_color.b = rand()%255;	
            
            if ( fixme.find(n) == fixme.end() ) 
            fixme[n]=z_color;
            
            m_renderer.set_drawing_color(fixme[n].r,fixme[n].g,fixme[n].b);
          
        }

  private:
      ZRender m_renderer;
      ZChannelRouter m_router;

      struct Color { int r; int g; int b; };
      std::map<ZNet*, Color> fixme;

};

#endif