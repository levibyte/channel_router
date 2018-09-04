#ifndef connectivity_h
#define connectivity_h


#include "objects.h"

#include <cassert>
#include <string>
#include <list>



class ZNet 
{
    
    public:
          //get_terms()
          ZNet (const std::string& str ):m_terms_count(0),m_name(str) { 
              m_farest_term = 0;
              m_closest_term = 0;
              is_first_term = true;
            
          }
          
          //fixme!
          /*
          void add_term(ZInst::ZInstTerm* instterm) {
	      assert(instterm);
	      add_term(instterm->col(),instterm->term()->row()?ZLowerTerm:ZUpperTerm,instterm->term()->name().c_str());
	  }*/
          
          //fixme!
          ZTerm* add_term(unsigned int c, ZTermOrientation o, const char* n = "?") {
              //std::cout << "Nearest:" << m_closest_term<< " Farest:" << m_farest_term << std::endl;
              
              ZTerm* t = new ZTerm(c,o,this,n);
              //terms.push_back(t);
	      //m_terms_count++;
	      
	      return add_term_base(t);
	  }
	  
          ZTerm* add_term_base(ZTerm* t) {
              terms.push_back(t);
	      m_terms_count++;
	      unsigned int c = t->col();
	      
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
          
          //bool operator < (const MyStruct& str) const {
          //    return (key < str.key);
          //}
    
	  unsigned int terms_count() {
	      return m_terms_count;
	  }
    private:
          //std::list<ZTerm*> terms;
         
          ZTerm* m_farest_term;
          ZTerm* m_closest_term;
          bool is_first_term;
          
          std::list<ZTerm*> terms;
	         unsigned int m_terms_count;
	  
          std::string m_name;
};



/*
class ZInstTerm 
{
    public:
	  ZInstTerm(ZInst* i,ZTerm* t):m_inst(i),m_term(t) {}
	  unsigned int col();
	  unsigned int row();
  
	  ZInst* inst();
	  ZTerm* term();
	  
    private:
	  ZInst* m_inst;
	  ZTerm* m_term;
};

unsigned int ZInstTerm::col() { return inst()->col() + term()->col(); }
unsigned int ZInstTerm::row() { return inst()->row() + term()->row(); }
ZInst* ZInstTerm::inst() { return m_inst; }
ZTerm* ZInstTerm::term() { return m_term; }	  

*/





#endif
