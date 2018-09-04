#ifndef netlist_helper_h
#define netlist_helper_h

#include "router.h"

class ZNetlisterHelper;

class ZRow1 { 
  public:  
        ZRow1(ZNetlisterHelper* n ):m_netlister(n) {
          
        }
        
  ZNetlisterHelper* m_netlister;
  
};


class ZRow2 { 
  public:  
        ZRow2(ZNetlisterHelper* n ):m_netlister(n) {
          
        }
        
  ZNetlisterHelper* m_netlister;
  
};

class ZRow3 { 
  public:  
        ZRow3(ZNetlisterHelper* n ):m_netlister(n) {
          
        }
        
  ZNetlisterHelper* m_netlister;
  
};


class ZRow4 { 
  public:  
        ZRow4(ZNetlisterHelper* n ):m_netlister(n) {
          
        }
        
  ZNetlisterHelper* m_netlister;
  
};

class ZNetlisterHelper 
{
  
    public:
    
           
    public:
            ZNetlisterHelper(ZChannelRouter* r):m_router(r) {
              //ZNetlisterHelper() {
              top_num = 0;
              buttom_num = 0;
	             m_router->terms_willbe_stored();
            }
            
            
            ZRow1 top_row() {
                //add_term(stream.str(),top_num,ZLowerTerm); 
                //top_num++;
                return ZRow1(this);
            }
            
            ZRow3 buttom_row()  {
                //add_term(name,buttom_num,ZUpperTerm); 
                //buttom_num++;
                return ZRow3(this);
            }
            
            void add_term1(const std::string& name) {
                // if (name.empty()) return;
                
                ZNet* net = m_router->get_or_create_net_by_name(name);
                m_router->add_term_to_net(net,top_num,ZUpperTerm); 
                //std::cout << " Adding buttom term " << name << " " << top_num << std::endl;
                top_num++;
            }

           void add_term2(const std::string& name) {
                 //if (name.empty()) return;
                 ZNet* net = m_router->get_or_create_net_by_name(name);
                 m_router->add_term_to_net(net,buttom_num,ZLowerTerm);
                
                  //std::cout << " Adding lower term " << name << " " << buttom_num << std::endl;
                  buttom_num++;

                 // std::cout << " Adding lower term " << name << std::endl;
            }
            
            /*
            void add_term(std::string name, unsigned int i, ZTermOrientation o) {
               if (name.empty()) return;
               ZNet* net = m_router->get_or_create_net(name);
               m_router->add_term_to_net(net,i,o); 
            }*/
            

            
          
    private:
         unsigned int top_num;
         unsigned int buttom_num;
         
         
         ZChannelRouter* m_router;
         
         
};



///ESOVAAARAAAA :D
namespace {
      
    ZRow1 operator << (ZRow2 D, std::string value) {
        //D.m_netlister->buttom_num++;
        D.m_netlister->add_term1(value);
        return ZRow1(D.m_netlister);
    }


    ZRow2 operator << (ZRow1 D, std::string value) {
        //D.m_netlister->buttom_num++;
        D.m_netlister->add_term1(value);
        return ZRow2(D.m_netlister);
    }


    ZRow3 operator << (ZRow4 D, std::string value) {
        //D.m_netlister->top_num++;
        D.m_netlister->add_term2(value);
        return ZRow3(D.m_netlister);
    }


    ZRow4 operator << (ZRow3 D, std::string value) {
        // D.m_netlister->top_num++;
        D.m_netlister->add_term2(value);
        return ZRow4(D.m_netlister);
    }
}

#endif
