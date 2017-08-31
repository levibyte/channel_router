Channel Router (VLSI)
===

What is this.
---

This is just-for-fun educational project to design, implement and render some of the channel routing algorithms, the most commonly used routing strategy for VLSI routing. There are several algorihtms used in channel routing,  and this "library" implements most popular ones: leftEdge and greedy routing.

Concept is having row of terminals seperated by "gap" called channel. 
Aim is route each connection in the chhanel so no vertial and horizontal lines overlap


Example.
---
_Input:_

     Upper Row: N0 N1 N4 N5 N1 N6 N7 N0 N4 N9 N10 N10

     Lower Row: N2 N3 N5 N3 N5 N2 N6 N8 N9 N8 N7 N9

_Ouput:_

<img src="https://image.ibb.co/dGNx45/5555.png">

Also it is possible to have metal view rendering ( leftclick ). 

Assuming tracks are on metal1 , segments are on metal2.

<img src="https://image.ibb.co/dDqs45/2222.png">

Acutal drawing ( interlayer.h ) is library independed, 
but current implementation is using SDL as base renderer.
Dependcy kept minimal , so it should be very easy to port this to Qt or to other library.

Code interfaces.
---

Input to the router should be nets, 
and where every net could have terminal assigned to lower or upper row.

Eficient way to provide input to the router is to use netlist helper utility class,

```
     #include "router.h"
     #include "netlist_helper.h"

     ZChannelRouter router;
     ZNetlisterHelper nh(&router);  
            nh.top_row()    << "N0" << "N1" << "N4" << "N5" << "N1" << "N6" << "N7" << "N0" << "N4" << "N9" << "N10" << "N10";
            nh.buttom_row() << "N2" << "N3" << "N5" << "N3" << "N5" << "N2" << "N6" << "N8" << "N9" << "N8" <<  "N7" << "N9";
      
    //router.load(nh.get());	      
    router.route();	    
```

or more "manual" way, adding each invidual net and it's corresponding terminal one by one.

```
     #include "router.h"

     ZChannelRouter router;
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

    router.add_net_to_route(a);
    router.add_net_to_route(b);
    router.add_net_to_route(c);
  
    router.route();
```

Copy/Paste code Example.
---
```
     #include "router.h"
     #include "netlist_helper.h"
     #include "interlayer.h"
     #include "connectivity.h"

     int main( int argc, char* args[] )
     {

          ZChannelRouter router;

          //rendering and router inerterlayer
          ZInterLayer il(&router);

          //helper
          ZNetlisterHelper nh(&router);  
          nh.top_row()    << "A" << "B" << "A" << "C";
          nh.buttom_row() << "B" << "C" << "C" << ;

          router.route();         

          // start process events
          il.start();    

          // close when quit
          il.end();

          return 0;
     }

```


Limitations.
---
Left edge algorithm can not be used if VCG containts cycle.

Dependencies.
---
libSDL2.a , included in the package



