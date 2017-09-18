#include "router.h"
#include "interlayer.h"


#include "example1.h"
#include "example2.h"
#include "example3.h"

//fixme put in other compilable module
ZRefManager* ZRefManager::instance = 0;


int main( int argc, char* args[] )
{
	
	ZChannelRouter* router = new ZLeftEdgeChannelRouter;
        ZInterLayer il(router);
      	
	//example1(router);
        example2(router);
        //example3(router);
        	
	router->route();
        il.start();    
        il.end();
        
return 0;
}

#ifdef OS_WINDOWS
int WinMain( int argc, char* args[] )
{
	//windows complains for undefinded symbol , this is stupid workaround
	main(argc,argv);
}
#endif
