#ifndef objects_h
#define objects_h

#include "connectivity.h"

#include <cassert>
#include <set>
#include <iostream>

const int CHANNEL_MAX=30;
enum ZTermOrientation { ZLowerTerm = CHANNEL_MAX, ZUpperTerm = 0  };
enum ZTermType { ZInputTerm, ZOutputTerm };


class ZTerm
{
    public:
            //ZTerm(ZTerm&)
	    ZTerm(unsigned int col,ZTermOrientation orient,ZNet* net):m_colnum(col),m_orient(orient),m_owner_net(net) {
                //std::cout << m_
            }
           
            ZTerm(unsigned int col,ZTermOrientation orient,ZNet* net,const char* name):m_colnum(col),m_orient(orient),m_owner_net(net),m_name(name) {
                //std::cout << m_
            }
           
            ZTerm(const char* name, unsigned int col,ZTermOrientation orient):m_name(name),m_colnum(col),m_orient(orient),m_owner_net(0) {
                //std::cout << m_
            }
                 
            ZNet* net() { assert(m_owner_net); return  m_owner_net; }     
            
            virtual unsigned int col() { return m_colnum; }
            virtual unsigned int row() { return m_orient; }
			 //draw_square(col_to_x(t->col()),row_to_y(!t->row()?0:m_router->get_maxtracks()+1),m_term_shape_rect_size);
			 
			 virtual unsigned int row1() { return !row()?0:(CHANNEL_MAX+1); }

			 virtual std::string name() { return m_name; }
            
            void set_name(const std::string& s) { 
	     // std::cout << " ZTERM: " << m_name << " -> " << s << std::endl;
	      m_name = s;
	      
	    }
            
    //private:
            unsigned int m_colnum;
            ZTermOrientation m_orient;
            ZNet* m_owner_net;
	    std::string m_name;
	    
            //unsigned int x;
            //unsigned int y;
};


class ZInst;
struct ZRef
{
      //fixme should be set
      virtual std::vector<ZTerm*> get_terms() = 0;
      virtual std::set<ZInst*> get_insts() = 0;
	  
      virtual void add_inst(ZInst* i) = 0;
      virtual unsigned int w() = 0;
      virtual unsigned int h() = 0;
      
      virtual void place_term(ZTermType tt, const char* n,unsigned int r, unsigned int c) = 0;
};

class ZRefManager {
    public:
	  static ZRefManager* get() { 
	    if(!instance) 
	      instance = new ZRefManager; 
	    return instance;
	  }

    public:
	  void add_ref(ZRef* r) { m_refs.insert(r); }
	  std::set<ZRef*> get_refs() { return m_refs; }
    
    private:
	    std::set<ZRef*> m_refs;
	    //std::set<ZInst*> m_inst; 
    private:	  
	 static ZRefManager* instance;
};

class ZInst;
class ZSymbolRef : public ZRef
{
    public:
	  ZSymbolRef(const char* nm, unsigned int w, unsigned int h):m_name(nm),m_h(h),m_w(w) { 
	    ZRefManager::get()->add_ref(this);
	    
	  }  
	  
	  void add_inst(ZInst* i) {
	    m_insts.insert(i);
	  }
	  void place_term(ZTermType tt, const char* n,unsigned int r, unsigned int fixmenotused) {
		ZTermOrientation o;
		(tt==ZInputTerm)?o=ZLowerTerm:o=ZUpperTerm;
		ZTerm* t = new ZTerm(n,r,o);
		//ZTerm t(n,r,o);
		m_terms.push_back(t);  
	  }
	  
	  std::vector<ZTerm*> get_terms() {
	      return m_terms;
	  }
	  
	  std::set<ZInst*> get_insts() {
	    return m_insts;
	  }
	
	 unsigned int w() { return m_w; }
	 unsigned int h() { return m_h; }
	 
	 
      
private:  
      
      std::string m_name;
      std::vector<ZTerm*> m_terms; 
      unsigned int m_h;
      unsigned int m_w;
      
      std::set<ZInst*> m_insts;
      
};

class ZInst
{
    public:
  
      class ZInstTerm : public ZTerm
      {
	  public:
		ZInstTerm(ZInst* i,ZTerm* t):ZTerm(*t) 
		{
		  m_inst = i;
		  
		  //:ZTerm(m_inst->name()+"/"+m_term->name(), unsigned int col,ZTermOrientation orient):
		  //m_inst(i),m_term(t) { 
		  //std::cout << "inst " << m_inst->name() << ":" << m_term->name() << std::endl;
		  m_name = m_inst->name()+"/"+ZTerm::name();
		}
		
		
		virtual unsigned int col() { return inst()->col() + ZTerm::col(); }
		virtual unsigned int row() { return inst()->row(); }
	
		virtual std::string name() { return m_name; }
		ZInst* inst() { return m_inst; }
		//ZTerm* term() { return m_term; }
		
	  private:
		ZInst* m_inst;
		//ZTerm* m_term;
		std::string m_name;
      };

    public:
	  ZInst(const char* n,ZRef* m, unsigned int c, unsigned int r):m_name(n),m_master(m),m_row(r),m_col(c) {
	      m_master->add_inst(this);
	      
	      std::vector<ZTerm*> terms = m->get_terms();
	      //std::cout << "terms" << n << "!" << terms.size() << std::endl;
	      for (std::vector<ZTerm*>::iterator i = terms.begin(); i!=terms.end(); ++i ) {
		//fixme , ZTERM	
		//ZTerm* t = new ZTerm(*i);
		ZInstTerm * inst_t = new ZInstTerm(this,*i);
		//t->set_name(inst_t->name());
		m_fixme_terms.push_back(inst_t);
		//fixme lookup name
		//std::cout << inst_t->name() << std::endl;
		//m_inst_terms[inst_t->name()] = inst_t;
	      }
	  }
	  
	  /*
	  ZInstTerm* get_term(const char* n) {  
	    //assert(m_inst_terms.find(n) )
	    std::string nm(m_name+"/"+n);
	    std::cout << " find " << nm << std::endl;
	    if ( m_inst_terms.find(nm.c_str()) != m_inst_terms.end() )
	    return m_inst_terms[nm.c_str()];
	    
	    assert(0);
	  }*/
	  
	  std::list<ZTerm*> get_terms() {  
	      return m_fixme_terms;
	    
	  }
	  
	  unsigned int col() { return m_col; }
	  unsigned int row() { return m_row; }
	  std::string name() { return m_name; }
	  ZRef* master() { return m_master; }
	  
    private:
	  unsigned int m_row;
	  unsigned int m_col;
	  ZRef* m_master;
	  std::string m_name;

	  std::list<ZTerm*> m_fixme_terms;
	  //std::map<std::string,ZInstTerm*> m_inst_terms;
};





#endif