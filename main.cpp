#include "router.h"
#include "netlist_helper.h"
#include "interlayer.h"

#include "connectivity.h"

void other_way(ZChannelRouter& router) {
  
    ZNet* a = new ZNet("A");
        a->add_term(0,z_upper_row);
        a->add_term(2,z_upper_row);
            
    ZNet* b = new ZNet("B");
        b->add_term(1,z_upper_row);
        b->add_term(0,z_lower_row);
        
    ZNet* c = new ZNet("C");
        c->add_term(3,z_upper_row);
        c->add_term(1,z_lower_row);
        c->add_term(2,z_lower_row);
        
        
  router.add_net_to_route(a);
  router.add_net_to_route(b);
  router.add_net_to_route(c);
  

}


void second_way(ZChannelRouter& router) {
    
    ZNetlisterHelper netlist(&router);  
            netlist.top_row()    << "N0"   << "N1" << "N4" << "N5" << "N1" << "N6" << "N7" << "N0"  << "N4" << "N9" << "N10" << "N10";
            netlist.buttom_row() << "N2" << "N3" << "N5" << "N3" << "N5" << "N2" << "N6" << "N8" << "N9" << "N8" <<  "N7" << "N9";
      
}

int main( int argc, char* args[] )
{
	
        ZChannelRouter router;
        ZInterLayer il(&router);
        
      
        //other_way(router);
        second_way(router);
        
        router.route();
        il.start();    
        il.end();
        
        return 0;
}