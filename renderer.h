#ifndef renderer_h
#define renderer_h

#include <cassert>
#include <iostream>

#include <SDL.h>



const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
class ZRender
{

  public:
	ZRender(const char* title) {
	   init(title);
           SDL_RenderSetScale(m_render,2.0,2.0);

	}

	//void* get_render() { return m_render; }
	
	void draw_point() { 
	    //draw_circle();
	}
	
	void pick_color_from_name(std::srtring n) {
            SDL_Color z_color;
            z_color.r = rand()%255;
            z_color.g = rand()%255;
            z_color.b = rand()%255;
            z_color.a = 255;
            
            if ( fixme.find(n) != fixme.end() ) 
            fixme[n]=z_color;
            
            SDL_SetRenderDrawColor(m_render, rand()%255,rand()%255,rand()%255,255 );
          
        }
        
        void draw_rect(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
          SDL_Rect rectToDraw = {x1,y1,x2,y2};
          //std::cout << rectToDraw.x << " " << rectToDraw.y << " " << rectToDraw.x+rectToDraw.w << " " << rectToDraw.y+rectToDraw.h << " " << std::endl;   
          SDL_RenderDrawRect(m_render,&rectToDraw);
          //SDL_RenderPresent( m_render );
           
        }
  
  //private:

	SDL_Window* m_window;
	SDL_Renderer* m_render;
        map<std::string, SDL_Color> fixme;

	bool init(const char* title)
	{
	      bool success = true;
	      
	      //Initialize SDL
	      if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	      {
		      printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		      success = false;
	      }
	      else
	      {
		      //Set texture filtering to linear
		      if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		      {
			      printf( "Warning: Linear texture filtering not enabled!" );
		      }

		      //Create window
		      m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		      if( m_window == NULL )
		      {
			      printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			      success = false;
		      }
		      else
		      {
			      //Create renderer for window
			      m_render = SDL_CreateRenderer( m_window, -1, 0 );
			      if( m_render == NULL )
			      {
				      printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				      success = false;
			      }
			      else
			      {
				      //Initialize renderer color
				      //SDL_SetRenderDrawColor( m_render, 0xFF, 0xFF, 0xFF, 0xFF );
				      
				      

				      //Initialize PNG loading
				      /*int imgFlags = IMG_INIT_PNG;
				      if( !( IMG_Init( imgFlags ) & imgFlags ) )
				      {
					      printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					      success = false;
				      }
				      */
			      }
		      }
	      }

	      return success;
	  }



	  void close()
	  {
		  //Destroy window        
		  SDL_DestroyRenderer( m_render );
		  SDL_DestroyWindow( m_window );
		  m_window = NULL;
		  m_render = NULL;

		  //Quit SDL subsystems
		  //IMG_Quit();
		  SDL_Quit();
	  }



	    void draw_text(const char* s, const SDL_Point& p )
	    {
		  /*
		  SDL_Color c = {0, 0, 0};  
		  SDL_Surface* surfaceMessage = TTF_RenderText_Solid(m_ttf_font, s.c_str(), c); 
		  SDL_Texture* Message = SDL_CreateTextureFromSurface(m_render, surfaceMessage); 

		  SDL_Rect Message_rect; 
		  Message_rect.x = p.x;  
		  Message_rect.y = p.y; 
		  Message_rect.w = 20; 
		  Message_rect.h = 20; 

		  SDL_RenderCopy(m_render, Message, NULL, &Message_rect); 
		  */
	      
	    }


	  void draw_line(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
	      /*SDL_Point a;
	      SDL_Point b;
	      a.x=x1;
	      a.y=y1;
	      
	      b.x=x2;
	      b.y=y2;
	      */
	  
	      //assert(0);
	      //std::cout << "draw line" << std::endl;
	      //SDL_RenderClear( m_render );
	      SDL_SetRenderDrawColor(m_render, 255,0,0,255 );
	      SDL_RenderDrawLine(m_render,x1,y1,x2,y2);
	      SDL_SetRenderDrawColor( m_render, 0xFF, 0xFF, 0xFF, 0xFF );
	      SDL_RenderPresent( m_render );
	  }
	  
	  void refresh() {
              //SDL_RenderClear( m_render );
              //SDL_SetRenderDrawColor(m_render, 255,0,0,255 );
              //SDL_RenderDrawLine(m_render,x1,y1,x2,y2);
              SDL_SetRenderDrawColor( m_render, 0xFF, 0xFF, 0xFF, 0xFF );
              SDL_RenderPresent( m_render );
          }
	  

	  void draw_circle(const SDL_Point& center, int radius, SDL_Color c)//, SDL_Color color)
	  {
	      //std::cout << "circle " << center.x << " " << center.y << std::endl;
	      SDL_SetRenderDrawColor(m_render, c.r,c.g,c.b,c.a);
	      for (int w = 0; w < radius * 2; w++)
	      {
		  for (int h = 0; h < radius * 2; h++)
		  {
		      int dx = radius - w; // horizontal offset
		      int dy = radius - h; // vertical offset
		      if ((dx*dx + dy*dy) <= (radius * radius))
		      {
			  SDL_RenderDrawPoint(m_render, center.x + dx, center.y + dy);
		      }
		  }
	      }
	      SDL_SetRenderDrawColor(m_render, 255,0,0,255 );
	  }

  
  
};


#endif