#ifndef connectivity_h
#define connectivity_h

#include <cassert>

const int CHANNEL_MAX=30;

enum ZTermOrientation { ZLowerTerm = CHANNEL_MAX, ZUpperTerm = 0  };

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
          
          //bool operator < (const MyStruct& str) const {
          //    return (key < str.key);
          //}
    
    private:
          //std::list<ZTerm*> terms;
         
          ZTerm* m_farest_term;
          ZTerm* m_closest_term;
          bool is_first_term;
          
          std::list<ZTerm*> terms;
          std::string m_name;
};








#endif
