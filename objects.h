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
      virtual std::vector<ZTerm*> get_terms() = 0;
      virtual void place_term(ZTermType tt, const char* n,unsigned int r, unsigned int c) = 0;
};

class ZSymbolRef : public ZRef
{
    public:
	  ZSymbolRef(const char* nm):m_name(nm) { }  
	  
	  void place_term(ZTermType tt, const char* n,unsigned int r, unsigned int fixmenotused) {
		ZTermOrientation o;
		(tt==ZInputTerm)?o=ZLowerTerm:o=ZUpperTerm;
		ZTerm* t = new ZTerm(n,r,o);
		m_terms.push_back(t);  
	  }
	  
	  std::vector<ZTerm*> get_terms() {
	      return m_terms;
	  }
	  
    private:  
      
      std::string m_name;
      std::vector<ZTerm*> m_terms; 
};

class ZInst
{
    public:
  
      class ZInstTerm 
      {
	  public:
		ZInstTerm(ZInst* i,ZTerm* t):m_inst(i),m_term(t) {}
		unsigned int col() { return inst()->col() + term()->col(); }
		//unsigned int row() { return term()->row(); }
	
		ZInst* inst() { return m_inst; }
		ZTerm* term() { return m_term; }
		
	  private:
		ZInst* m_inst;
		ZTerm* m_term;
      };

    public:
	  ZInst(const char* n,ZRef* m, unsigned int c, unsigned int r):m_name(n),m_master(m),m_row(r),m_col(c) {
	      std::vector<ZTerm*> terms = m->get_terms();
	      for (std::vector<ZTerm*>::iterator i = terms.begin(); i!=terms.end(); ++i ) {
		ZInstTerm * inst_t = new ZInstTerm(this,*i);
		m_inst_terms[(*i)->name()] = inst_t;
	      }
	  }
	  
	  ZInstTerm* get_term(const char* n) {  
	    //assert(m_inst_terms.find(n) )
	    if ( m_inst_terms.find(n) != m_inst_terms.end() )
	    return m_inst_terms[n];
	    
	    assert(0);
	  };
	  
	  unsigned int col() { return m_col; }
	  unsigned int row() { return m_row; }
	  ZRef* master() { return m_master; }
	  
    private:
	  unsigned int m_row;
	  unsigned int m_col;
	  ZRef* m_master;
	  std::string m_name;
	  
	  std::map<std::string,ZInstTerm*> m_inst_terms;
};





#endif