#include "renderer.h"
#include "logic.h"

#include <SDL.h>


int main( int argc, char* args[] )
{

	ZRender renderer("ChannelRouter");
        ZChannelRouter router;
        ZInterLayer il(renderer,&router);
        
        ZNetlister netlist(&router);  
               //netlist.top_row()    << ""   << "N1" << "N4" << "N5" << "N6" << ""   << "N7" << "N4" << "N9" << "Ny" << "Ny" << "Z";
               //netlist.buttom_row() << "N2" << "N3" << "N5" << "N3" << "N5" << "N2" << "N6" << "N8" << "N9" << "N8" << "N7" << "N9";
               
               
                  netlist.top_row() << "A" << "B" << "A" << "C";
	       netlist.buttom_row() << "B" << "C" << "C" << "";
         
	
	router.route();
	

	
        unsigned int lastTime = 0, currentTime;

	SDL_Event e;
	bool quit = false;
        while( !quit )
        {
                currentTime = SDL_GetTicks();
                if (currentTime > lastTime + 100) {

		    while( SDL_PollEvent( &e ) != 0 )
		    {
			    if ( e.type == SDL_QUIT ) quit = true;
			    if ( e.type == SDL_MOUSEBUTTONDOWN ) il.change_colors();
			    //if ( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {

			    //for dbg
			    //if ( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_u) {
				//j->undo_permute();
			    //}
		    } 
		    
		    il.draw();
		    //renderer.draw_rect(10,10,10,10);
		    lastTime = currentTime;  
		}
        }

        renderer.close();
        return 0;
}