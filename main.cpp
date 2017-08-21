#include "renderer.h"
#include "logic.h"

#include <SDL.h>


int main( int argc, char* args[] )
{

	ZRender renderer("ChannelRouter");
	ZChannelRouter router;
	router.route();
	ZInterLayer il(renderer,router);

	
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