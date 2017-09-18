#ifndef interlayer_h
#define interlayer_h

#include "router.h"
#include "renderer.h"

#include <vector>
#include <map>
#include <list>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>

class ZInterLayer : public ZRender {
  
  public:
         ZInterLayer (ZChannelRouter* ro):m_metalmode(false),m_router(ro) {
            //ZRenderer re(this) = m_renderer;
            
         }
         
         void start() {
              ZRender::enter_event_loop();
         }
         
         void end() {
              ZRender::close();
         }
         
         virtual void draw() {
              //draw_tracks();
              //draw_nets(); 
              
              draw_insts();
              refresh();
          }
          
         
         virtual void notify_mouse_pressed(unsigned int btn) {
	   if ( 1 == btn )  
	     m_metalmode = false,change_colors();
	   else
	     m_metalmode = true;
         }
          
          void change_colors() {
              for(std::map<ZNet*,ZColor>::iterator i=m_net2color.begin();i!=m_net2color.end();++i)
                m_net2color[(*i).first] = get_rand_color();
          }

  private:
	  //fixme ? clever?
          void draw_insts() {
		  set_drawing_color(255,255,255);

		  std::set<ZRef*> refs = ZRefManager::get()->get_refs();
		  for ( std::set<ZRef*>::iterator r = refs.begin(); r!= refs.end(); ++r) {
		    std::set<ZInst*> insts = (*r)->get_insts();
		    for ( std::set<ZInst*>::iterator i = insts.begin(); i!= insts.end(); ++i ) {
			draw_rect(col_to_x((*i)->col()),row_to_y((*i)->row()),col_to_x((*r)->w()),row_to_y((*r)->h()));
			draw_terms((*i)->get_terms());
		    }
		  }
	  }
	  
	  void draw_tracks() {
              set_drawing_color(10,2,2);
              for(unsigned int i=m_router->get_maxtracks();i>0;i--) 
                draw_line(0,row_to_y(i),400,row_to_y(i));
          }
          
          void draw_nets() {
              std::list<ZNet*> nets = m_router->get_nets();
              for(std::list<ZNet*>::iterator i=nets.begin();i!=nets.end();++i) 
                if(!(*i)->get_name().empty()) 
                  draw_individual_net(*i);  
          }
          
          void draw_individual_net(ZNet* net) {
              m_metalmode?set_drawing_color(255,255,0):pick_color_for_net(net);
              
              draw_terms(net->get_terms());
              if ( m_router->is_done() ) 
                draw_routed_segments(net);
          }
         
          void draw_terms(const std::list<ZTerm*>& terms) {
              std::list<ZTerm*>::const_iterator j; 
              for(j=terms.begin();j!=terms.end();++j) {
                draw_term(*j);
                //draw_extensions(*j);
              }
          }

          
          void draw_term(ZTerm* t) {
              assert(m_router);
	      //std::cout << " drawterm " << t->name() << " : " << t->col() << "---" <<  t->row() << std::endl;
	      //draw_rect(20*t->col()+20,100*t->row()+20, 10, 10);
              
              //TODO
              //draw_pin(t->get_pin_shape());
	      
	      
	      int term_shape_rect_size = 3;
              draw_square(col_to_x(t->col()),row_to_y(!t->row()?0:m_router
              ->get_maxtracks()+1),term_shape_rect_size);
	      //fixme term_size
	      
	      //fixme t owner inst
	      //std::string (t->name()+t->)
	      draw_text(t->name().c_str(),col_to_x(t->col()),row_to_y(!t->row()?0:m_router
              ->get_maxtracks()+1));
          }

          void draw_routed_segments(ZNet* n) {
              if (m_router->get_net_track(n))
		draw_net_trunk_on_track(n);
              //draw_extensions(n);
              //draw_intersection_points();
          }
          
          void draw_net_trunk_on_track(ZNet* n) {
              unsigned int t = m_router->get_net_track(n);
              unsigned int c1 = n->get_closest_term()->col();
              unsigned int c2 = n->get_farest_term()->col();

              if( !m_metalmode )
		set_drawing_color(m_net2color[n].r,m_net2color[n].g,m_net2color[n].b);
              else
		set_drawing_color(0,0,255);
		
	      //draw_line(0,10*i+50,20+300,10*i+50);

              //std::cout << "Net:" << n->get_name() << " track:" << t << "  (" << c1 << "->" << c2 << ")" << std::endl;
              //20*t->col()+20,100*t->row()+20
              
              draw_line(col_to_x(c1),row_to_y(t),col_to_x(c2),row_to_y(t));
             
          }
          
          
          void draw_extensions(ZTerm* t) {
            if ( m_router->is_done() && ! t->net()->get_name().empty() && m_router->get_net_track(t->net()) )  {
              int tmp1 = row_to_y(!t->row()?0:m_router->get_maxtracks()+1);
              int tmp2 = row_to_y(m_router->get_net_track(t->net()));
              draw_line(col_to_x(t->col()),tmp1,col_to_x(t->col()),tmp2);
              if( t->net()->terms_count() > 2 )
		draw_point(col_to_x(t->col()),tmp2,2);
            }
         }


  private:
         unsigned int col_to_x(unsigned int col) {
            return 20*col+20;
         }
         
         unsigned int row_to_y(unsigned int o) {
           return 20*o+20;
         }
         

        struct ZColor { int r; int g; int b; };

         ZColor get_rand_color() {
            ZColor z_color;
            z_color.r = rand()%255;
            z_color.g = rand()%255;
            z_color.b = rand()%255;     

            return z_color;
         }

    
         /*
         int hash_get(const std::string& str){
                int hash = 5381;
                for (int i = 0; i < str.size(); i++) {
                  hash = ((hash << 5) + hash) + (int)str[i];
                }
                
          return hash;
          }


          ZColor hash_color(const std::string& str) {
            ZColor z_color;
            int hash = hash_get(str);
            z_color.r = (hash & 0xFF0000) >> 16;// = rand()%255;
            z_color.g = (hash & 0xFF0000) >> 8 ;// = rand()%255;
            z_color.b = (hash & 0xFF0000); // = rand()%255;     
            std::cout << str << "->" << hash << " " << z_color.r << std::endl;
            
            //std::cout << z_color.r << std::endl;
            //std::cout << z_color.g << std::endl;
            //std::cout << z_color.b << std::endl;
            //std::cout << std::endl;
            
            return z_color;
         }
        */
         
         void pick_color_for_net(ZNet* n) {
            
            if ( m_net2color.find(n) == m_net2color.end() ) {
              ZColor z_color = get_rand_color();//hash_color(n->get_name()); //
              m_net2color[n]=z_color;
            }
            
            set_drawing_color(m_net2color[n].r,m_net2color[n].g,m_net2color[n].b);
        }

  private:
      //ZRender m_renderer;
      ZChannelRouter* m_router;
      std::map<ZNet*, ZColor> m_net2color;

      bool m_metalmode;
};

#endif