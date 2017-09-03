#include "router.h"
#include "objects.h"

void create_connection(ZChannelRouter* router, const char* netname, ZInst::ZInstTerm* it1, ZInst::ZInstTerm* it2) {
    ZNet* net = new ZNet(netname);
        net->add_term(it1);
        net->add_term(it2);  
    
    router->add_net_to_route(net);
}

void inst_tintirit (ZChannelRouter* router) {
  
    ZSymbolRef* aoi3 = new ZSymbolRef("AOI3");
    //aoi3->place_term(ZInputTerm,"A",2,0);
    //aoi3->place_term(ZInputTerm,"B",4,0);
    //aoi3->place_term(ZInputTerm,"C",6,0);
    aoi3->place_term(ZOutputTerm,"Y",4,7777);
    
    ZSymbolRef* xor6 = new ZSymbolRef("XOR6");
    xor6->place_term(ZInputTerm,"A",2,7777);
    xor6->place_term(ZInputTerm,"B",4,7777);
    //xor6->place_term(ZInputTerm,"C",6,0);
    //xor6->place_term(ZInputTerm,"D",8,0);
    //xor6->place_term(ZInputTerm,"E",10,0);
    //xor6->place_term(ZInputTerm,"F",12,0);   
    //xor6->place_term(ZOutputTerm,"Y",8,1);
    
    ZInst* i1 = new ZInst("frommaster1",aoi3,0,0);
    ZInst* i2 = new ZInst("frommaster2",aoi3,10,0);
    //ZInst* i3 = new ZInst("frommaster3",aoi3,4,0);
    //ZInst* i4 = new ZInst("frommaster3",aoi3,6,0);
    //ZInst* i5 = new ZInst("frommaster4",aoi3,8,0);
    //ZInst* i6 = new ZInst("frommaster5",aoi3,10,0);
    
    ZInst* i7 = new ZInst("frommaster6",xor6,12,1);
    
    
    create_connection(router,"net1",i1->get_term("Y"),i7->get_term("A"));
    //create_connection(router,"net2",i2->get_term("Y"),i7->get_term("B"));
    //create_connection(router,"net3",i3->get_term("Y"),i7->get_term("C"));
    //create_connection(router,"net4",i4->get_term("Y"),i7->get_term("D"));
    //create_connection(router,"net5",i5->get_term("Y"),i7->get_term("E"));
    //create_connection(router,"net6",i6->get_term("Y"),i7->get_term("F"));  
}


void route_schematic_design1(ZChannelRouter* router) {
	inst_tintirit(router);
	router->route();
}

