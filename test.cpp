#include <SDL.h>

#include <vector>
#include <map>
#include <list>

class ZNet;

class ZRender
{

  public:
	ZRender() {
	  
	}

	void* get_render() { return m_render; }
	
	void draw_net(const ZNet& n) {
	  //draw_trunk(n);
	  //draw_segments(n);    
	}
  
  private:
	void* m_render;
};


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
	  void add_term(unsigned int c, ZTermOrientation o) {
	      ZTerm t(c,o);
	    
	  }
	  
	  void assign_track(unsigned int n) { m_track_num = n; }
	  
    private:
	  std::list<ZTerm*> terms;
	  unsigned int m_track_num;//std::list<>
      
};



int main()
{
    ZNet a,b,c;
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
    
    ZRender r;
    r.draw_net(a);
    r.draw_net(b);
    r.draw_net(c);
    
  
}