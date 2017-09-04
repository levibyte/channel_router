#include "router.h"
#include "netlist_helper.h"
#include "interlayer.h"

#include "connectivity.h"
#include "objects.h"

/*
void manual_way(ZChannelRouter* router) {
  
    ZNet* a = new ZNet("A");
        a->add_term(0,ZUpperTerm);
        a->add_term(2,ZUpperTerm);
            
    ZNet* b = new ZNet("B");
        b->add_term(1,ZUpperTerm);
        b->add_term(0,ZLowerTerm);
        
    ZNet* c = new ZNet("C");
        c->add_term(3,ZUpperTerm);
        c->add_term(1,ZLowerTerm);
        c->add_term(2,ZLowerTerm);

    router->add_net_to_route(a)
    router->add_net_to_route(b);
    router->add_net_to_route(c);
  
    router->route();

}

  
void route_schematic_design(ZChannelRouter* router) {
  
    ZNet* a = new ZNet("A");
        a->add_term(0,ZUpperTerm);
        a->add_term(2,ZLowerTerm);
            
    ZNet* b = new ZNet("B");
        b->add_term(10,ZUpperTerm);
        b->add_term(4,ZLowerTerm);
        
    ZNet* c = new ZNet("C");
        c->add_term(20,ZUpperTerm);
        c->add_term(6,ZLowerTerm);

    ZNet* d = new ZNet("D");
        d->add_term(40,ZUpperTerm);
        d->add_term(8,ZLowerTerm);

    ZNet* e = new ZNet("E");
        e->add_term(60,ZUpperTerm);
        e->add_term(10,ZLowerTerm);

	
    router->add_net_to_route(a);
    router->add_net_to_route(b);
    router->add_net_to_route(c);
    router->add_net_to_route(d);
    router->add_net_to_route(e);
  
    router->route();

}


void automated_way(ZChannelRouter* router) {
    
    ZNetlisterHelper nh(router);  
            nh.top_row()    << "N0" << "N1" << "N4" << "N5" << "N1" << "N6" << "N7" << "N0" << "N4" << "N9" << "N10" << "N10";
            nh.buttom_row() << "N2" << "N3" << "N5" << "N3" << "N5" << "N2" << "N6" << "N8" << "N9" << "N8" <<  "N7" << "N9";
      
            //nh.top_row()    << "" << "A" << "" << "B" << "" << "C";
            //nh.buttom_row() << "" << "" << "A" << "" << "B" "" << "" << "C";
    
	    
    //router.load(nh.get());	      
    router->route();	    
}
*/



#include "example1.h"

int main( int argc, char* args[] )
{
	
        //ZChannelRouter* router = new ZGreedyChannelRouter;
	//ZChannelRouter* router = new ZSchematicChannelRouter;
	ZChannelRouter* router = new ZLeftEdgeChannelRouter;
	
	
        ZInterLayer il(router);
        
      
        //manual_way(router);
        //automated_way(router);
        route_schematic_design1(router);
	  
        il.start();    
        il.end();
        
        return 0;
}