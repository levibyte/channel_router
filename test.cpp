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
	    
	    void draw() {
	      
	      
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
	  ZNet (const char* str):m_name(str),m_track_num(0) { 
	    
	  }
	  
	  void add_term(unsigned int c, ZTermOrientation o) {
	      ZTerm t(c,o);
	    
	  }
	  
	  void assign_track(unsigned int n) { m_track_num = n; }
	  
    private:
	  std::list<ZTerm*> terms;
	  unsigned int m_track_num;//std::list<>
	  std::string m_name;
};



class ZChannelRouter {

  public:
	
    ZChannelRouter():maxtracks(6) {
	    ZNet a("A"),b("B"),c("C");
	    a.add_term(0,z_upper_row);
	    a.add_term(2,z_upper_row);
	    a.assign_track(1);
	    
	    b.add_term(1,z_upper_row);
	    b.add_term(0,z_lower_row);
	    b.assign_track(2);
	    
	    c.add_term(3,z_upper_row);
	    c.add_term(1,z_lower_row);
	    c.add_term(2,z_lower_row);
	    c.assign_track(3);
	}
	
	void route() {
	  
	  
	}
	
	void draw_terms() {
	  // foreach net, foreach term,
	  t.draw();
	}
	
	void draw_nets() {
	  
	}
	
  
  private:
	 std::map<ZNet,unsigned int> m_net2track;
	 unsigned int maxtracks;
  
};


int main()
{
    ZRender renderer;
    ZChannelRouter router(renderer);
    
    router.draw_terms()
    router.route();
    router.draw_nets()
    
  
}