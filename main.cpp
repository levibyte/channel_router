#include "renderer.h"
#include "logic.h"

#include <map>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <iostream>

#include <SDL.h>

int main( int argc, char* args[] )
{

	JRenderer renderer("Simulated annealing placement test");
        SDL_Event e;
        JManager* j = new JManager(&renderer);

        unsigned int lastTime = 0, currentTime;
        //SDL_RenderSetScale(gRenderer,6.0,6.0);

	bool quit = false;
        while( !quit )
        {
                currentTime = SDL_GetTicks();
                if (currentTime > lastTime + 100) {

		    while( SDL_PollEvent( &e ) != 0 )
		    {
			    if ( e.type == SDL_QUIT ) quit = true;
			    if ( e.type == SDL_MOUSEBUTTONDOWN ) j->action();
			    if ( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
				if(!j) delete j; 
				j = new JManager(&renderer);
			    }

			    //for dbg
			    //if ( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_u) {
				//j->undo_permute();
			    //}
			    
			    
		    } 
	
		    j->do_and_draw();
		    lastTime = currentTime;  
		}
        }

        renderer.close();
        return 0;
}