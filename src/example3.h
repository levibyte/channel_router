#include "router.h"
#include "connectivity.h"

void example1(ZChannelRouter* router) {
  
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

    router->add_net_to_route(a);
    router->add_net_to_route(b);
    router->add_net_to_route(c);
  
    router->route();

}
