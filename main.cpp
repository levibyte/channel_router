#include "renderer.h"
#include "logic.h"

#include <SDL.h>


#include <sstream>
#include <cassert>



int main( int argc, char* args[] )
{

	ZRender renderer("ChannelRouter");
        ZChannelRouter router;
        ZInterLayer il(renderer,&router);
        
        ZNetlister netlist(&router);  
               netlist.top_row() << "A" << "B" << "A" << "C" << "D" << "E" << "A" << "A" << "A";
               netlist.buttom_row() << "B" << "C" << "C" << "" << "D";
         
	
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