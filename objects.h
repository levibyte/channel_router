#ifndef objects_h
#define objects_h

#include "connectivity.h"

#include <cassert>

const int CHANNEL_MAX=30;
enum ZTermOrientation { ZLowerTerm = CHANNEL_MAX, ZUpperTerm = 0  };
enum ZTermType { ZInputTerm, ZOutputTerm };


class ZTerm
{
    public:
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
            unsigned int col() { return m_colnum; }
            unsigned int row() { return m_orient; }
            std::string name() { return m_name; }
            void set_name(const std::string& s) { 
	      std::cout << " ZTERM: " << m_name << " -> " << s << std::endl;
	      m_name = s;
	      
	    }
            
    private:
            unsigned int m_colnum;
            ZTermOrientation m_orient;
            ZNet* m_owner_net;
	    std::string m_name;
	    
            //unsigned int x;
            //unsigned int y;
};



struct ZRef
{
      //fixme should be set
      virtual std::vector<ZTerm> get_terms() = 0;
      virtual void place_term(ZTermType tt, const char* n,unsigned int r, unsigned int c) = 0;
};

class ZSymbolRef : public ZRef
{
    public:
	  ZSymbolRef(const char* nm):m_name(nm) { }  
	  
	  void place_term(ZTermType tt, const char* n,unsigned int r, unsigned int fixmenotused) {
		ZTermOrientation o;
		(tt==ZInputTerm)?o=ZLowerTerm:o=ZUpperTerm;
		//ZTerm* t = new ZTerm(n,r,o);
		ZTerm t(n,r,o);
		m_terms.push_back(t);  
	  }
	  
	  std::vector<ZTerm> get_terms() {
	      return m_terms;
	  }
	  
    private:  
      
      std::string m_name;
      std::vector<ZTerm> m_terms; 
};

class ZInst
{
    public:
  
      class ZInstTerm 
      {
	  public:
		ZInstTerm(ZInst* i,ZTerm* t):m_inst(i),m_term(t) { 
		  //std::cout << "inst " << m_inst->name() << ":" << m_term->name() << std::endl;
		  m_name = m_inst->name()+"/"+m_term->name();
		  
		}
		unsigned int col() { return inst()->col() + term()->col(); }
		//unsigned int row() { return term()->row(); }
	
		std::string name() { return m_name; }
		ZInst* inst() { return m_inst; }
		ZTerm* term() { return m_term; }
		
	  private:
		ZInst* m_inst;
		ZTerm* m_term;
		std::string m_name;
      };

    public:
	  ZInst(const char* n,ZRef* m, unsigned int c, unsigned int r):m_name(n),m_master(m),m_row(r),m_col(c) {
	      std::vector<ZTerm> terms = m->get_terms();
	      std::cout << "terms" << n << "!" << terms.size() << std::endl;
	      for (std::vector<ZTerm>::iterator i = terms.begin(); i!=terms.end(); ++i ) {
		//fixme , ZTERM	
		ZTerm* t = new ZTerm(*i);
		ZInstTerm * inst_t = new ZInstTerm(this,t);
		t->set_name(inst_t->name());
		
		//fixme lookup name
		//std::cout << inst_t->name() << std::endl;
		m_inst_terms[inst_t->name()] = inst_t;
	      }
	  }
	  
	  ZInstTerm* get_term(const char* n) {  
	    //assert(m_inst_terms.find(n) )
	    std::string nm(m_name+"/"+n);
	    std::cout << " find " << nm << std::endl;
	    if ( m_inst_terms.find(nm.c_str()) != m_inst_terms.end() )
	    return m_inst_terms[nm.c_str()];
	    
	    assert(0);
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
	  
	  std::map<std::string,ZInstTerm*> m_inst_terms;
};





#endif