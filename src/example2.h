#include "netlist_helper.h"
#include "router.h"

void example2(ZChannelRouter* router) {
    
    ZNetlisterHelper nh(router);  
            nh.top_row()    << "N0" << "N1" << "N4" << "N5" << "N1" << "N6" << "N7" << "N0" << "N4" << "N9" << "N10" << "N10";
            nh.buttom_row() << "N2" << "N3" << "N5" << "N3" << "N5" << "N2" << "N6" << "N8" << "N9" << "N8" <<  "N7" << "N9";
      
            //nh.top_row()    << "" << "A" << "" << "B" << "" << "C";
            //nh.buttom_row() << "" << "" << "A" << "" << "B" "" << "" << "C";
    
	    
    //router.load(nh.get());	      
    //router->route();	    
}