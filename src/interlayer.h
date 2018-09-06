#ifndef interlayer_h
#define interlayer_h

#include "router.h"
#include "renderer.h"

#include <vector>
#include <map>
#include <list>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>

class ZInterLayer : public ZRender {
  
  public:
         ZInterLayer (ZChannelRouter* ro):m_metalmode(false),m_router(ro) {
            //ZRenderer re(this) = m_renderer;
			 m_render_multip = 120;
			 m_render_const_factor = m_render_multip;
			 m_render_const_delta_x =  m_render_multip;
			 m_render_const_delta_y =  m_render_multip;
			 
                         m_term_shape_rect_size = 15;
                         m_extension_point_radius = 10;
			 m_active_term = 0;
			 m_highlight_thickness = 10;
			 m_draw_rest_dimmed = false;
			 
             std::list<ZTerm*>::const_iterator j ;
             std::list<ZNet*> nets = m_router->get_nets();
             for(std::list<ZNet*>::iterator i=nets.begin();i!=nets.end();++i) {
                 std::list<ZTerm *> terms = (*i)->get_terms();
                 for (j = terms.begin(); j != terms.end(); ++j) {
                     m_all_terms.push_back(*j);
					 //std::cout << (*j)->net()->get_name() << "  " <<  (*j)->col() << " " << (*j)->row1() << std::endl;
					 //std::cout << "ARE:" << col_to_x((*j)->col()) << " " << row_to_y((*j)->row1()) << std::endl;
					 //std::cout<< std::endl;
                 }
				  
             }
			  ////assert(0);
			////std::cout << "All terms: " << m_all_terms.size() << std::endl;
			////std::cout << "All nets: " << m_router->get_nets().size() << std::endl;
			
		 }


		 void dump_data() {
             std::list<ZTerm*>::const_iterator j ;
             std::list<ZNet*> nets = m_router->get_nets();
             for(std::list<ZNet*>::iterator i=nets.begin();i!=nets.end();++i) {
                 std::list<ZTerm *> terms = (*i)->get_terms();
                 for (j = terms.begin(); j != terms.end(); ++j) {
                     m_all_terms.push_back(*j);
					 std::cout << (*j)->net()->get_name() << "  " <<  (*j)->col() << " " << (*j)->row1() << std::endl;
					 std::cout << "ARE:" << col_to_x((*j)->col()) << " " << row_to_y((*j)->row1()) << std::endl;
					 std::cout<< std::endl;
                 }
				  
             }

		 }
		 
         void check_select_active_object(float x, float y){
                //set_drawing_color(10,2,2);
                //draw_point(x,y, 50);

               check_for_term(x, y);
         }


        void check_for_term(float x, float y) {
            for(std::vector<ZTerm*>::iterator i=m_all_terms.begin();i!=m_all_terms.end();++i) {
                //std::cout << (*i)->net()->get_name() << " 1" << std::endl;
				bool f = is_in_term_bounds(x,row_to_y(!(*i)->row()?0:m_router->get_maxtracks()+1));
				//std::cout << (*i)->net()->get_name() << " 2" << std::endl;
			    bool s = is_in_term_bounds(y,col_to_x((*i)->col()));
			//std::cout << std::endl;
			//std::cout << std::endl;
				
                ////std::cout << f << " " << s << std::endl;
                if ( f && s ) {
                /////std::cout << "Highlight term: " << (*i)->net()->get_name() << std::endl;
					m_active_term = *i;
					m_draw_rest_dimmed = true;
					//std::cout << std::endl;
			//std::cout << std::endl;
			
                    return;
                }
                }
			m_active_term = 0;
			m_draw_rest_dimmed = false;
			//std::cout << std::endl;
			//std::cout << std::endl;
			
        }

		void highlight_term(ZTerm* t) {
			set_drawing_color(255,255,0);
			std::list<ZTerm*> all_terms = t->net()->get_terms();
			for(std::list<ZTerm*>::iterator i=all_terms.begin();i!=all_terms.end();++i) {
				//std::cout << "ARE:" << col_to_x((*i)->col()) << " " << row_to_y((*i)->row1()) << std::endl;
				draw_square(col_to_x((*i)->col()),row_to_y(!(*i)->row()?0:m_router->get_maxtracks()+1),m_term_shape_rect_size+m_highlight_thickness);
			}
			
			ZNet* n = (t)->net();
			set_drawing_color(get_net_color(n).r,get_net_color(n).g,get_net_color(n).b);
			draw_square(col_to_x(t->col()),row_to_y(t->row1()),m_term_shape_rect_size);
			
			////std::cout << "TERM UNDER MOUSE" << std::endl;		
		}
		
        bool is_in_term_bounds( int a ,  int  b) {
            ////std::cout << "---" << (a < (b + m_term_shape_rect_size))  << " - " <<  (a >= (b - m_term_shape_rect_size)) << std::endl;
            //std::cout << "isbound" << a << " " << b << std::endl;
			return ( (a < (b + m_term_shape_rect_size)) &&  (a >= (b - m_term_shape_rect_size)) );
            

        }

         void zoom_in() {
			 // TODO //FIXME do range check
             m_render_const_factor /=2;
             m_render_const_delta_x /=2;
             m_render_const_delta_y /=2;
                        
             m_term_shape_rect_size /=2;
			 m_highlight_thickness /=2;
             m_extension_point_radius /=2;
			 dump_data();
         }

        void zoom_out() {
			 // TODO //FIXME do range check

		m_render_const_factor *= 2;
            //m_render_const_delta *= 2;
			m_render_const_delta_x *= 2;
			m_render_const_delta_y *= 2;
			m_term_shape_rect_size *= 2;
            m_extension_point_radius *= 2;
			m_highlight_thickness *=2;
			dump_data();
        }

        void zoom_factor() {

        }

         void start() {
              ZRender::enter_event_loop();
         }
         
         void end() {
              ZRender::close();
         }
         

         virtual void draw() {

             ////std::cout << "drawing" << std::endl;
             //std::cerr << "drawing" << std::endl;
             //SDL_Log("aaaa");

			
			  //draw_tracks();
              draw_nets(); 
              //draw_insts();
			  draw_highlights();

              refresh();
          }
          
         
        virtual void notify_mouse_pressed(unsigned int btn) {
            m_all_terms[rand()%m_all_terms.size()-1]->m_colnum = rand()%10;
            /*
            if ( 1 == btn )
             m_metalmode = false,change_colors();
                else
             m_metalmode = true;
            */
            
         }
          
          void change_colors() {
              for(std::map<ZNet*,ZColor>::iterator i=m_net2color.begin();i!=m_net2color.end();++i)
                m_net2color[(*i).first] = get_rand_color();
          }

  private:
	  //fixme ? clever?
      void draw_insts() {
		  set_drawing_color(255,255,255);
/*
		  std::set<ZRef*> refs = ZRefManager::get()->get_refs();
		  for ( std::set<ZRef*>::iterator r = refs.begin(); r!= refs.end(); ++r) {
		    std::set<ZInst*> insts = (*r)->get_insts();
		    for ( std::set<ZInst*>::iterator i = insts.begin(); i!= insts.end(); ++i ) {
			draw_rect(col_to_x((*i)->col()),row_to_y((*i)->row1()),col_to_x((*r)->w()),row_to_y((*r)->h()));
			draw_terms((*i)->get_terms());
		    }
		  }*/
	  }
	  		
		virtual void zoomin_to_point(float x,float y) {
			zoom_in();
			m_render_const_delta_y = x;
			m_render_const_delta_y = y;
			
		}
		
		
		virtual void zoomout_to_point(float x,float y) {
			zoom_out();
			m_render_const_delta_x = x;
			m_render_const_delta_y = y;
		}
	
	  void draw_highlights() {
			////std::cout << "drawing highlight with active term" << m_active_term << std::endl;
			if (m_active_term) {
				highlight_term(m_active_term);	
				m_draw_rest_dimmed = false;
				draw_individual_net(m_active_term->net());
				m_draw_rest_dimmed = true;
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
                draw_extensions(*j);
              }
          }

          
          void draw_term(ZTerm* t) {
            assert(m_router);
			draw_square(col_to_x(t->col()),row_to_y(!t->row()?0:m_router->get_maxtracks()+1),m_term_shape_rect_size);
           // draw_square(col_to_x(t->col()),row_to_y(t->row1()),m_term_shape_rect_size);
            //draw_text(t->name().c_str(),col_to_x(t->col()),row_to_y(t->row1()));
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
		        set_drawing_color(get_net_color(n).r,get_net_color(n).g,get_net_color(n).b);
              else
	        	set_drawing_color(0,0,255);
		
	             //draw_line(0,10*i+50,20+300,10*i+50);

              ////std::cout << "Net:" << n->get_name() << " track:" << t << "  (" << c1 << "->" << c2 << ")" << std::endl;
              //20*t->col()+20,100*t->row1()+20
              
              draw_line(col_to_x(c1),row_to_y(t),col_to_x(c2),row_to_y(t));
             
          }
          
          
          void draw_extensions(ZTerm* t) {
            if ( m_router->is_done() && ! t->net()->get_name().empty() && m_router->get_net_track(t->net()) )  {
              int tmp1 = row_to_y(!t->row()?0:m_router->get_maxtracks()+1);
              int tmp2 = row_to_y(m_router->get_net_track(t->net()));
              draw_line(col_to_x(t->col()),tmp1,col_to_x(t->col()),tmp2);
              if( t->net()->terms_count() > 2 )
		        draw_point(col_to_x(t->col()),tmp2,m_extension_point_radius);
            }
         }


 
		void pan_left() {
			m_render_const_delta_y/= 2;
		}
		
		void pan_right() {
			m_render_const_delta_y*= 2;
			
		}
		
		void pan_down() {
		    m_render_const_delta_x*= 2;		 
			
		}
		
		void pan_up() {
		    m_render_const_delta_x/= 2;		 
			
		}
		
 private:
         
         inline unsigned int col_to_x(unsigned int col) {
            return m_render_const_factor*col+m_render_const_delta_x;
         }
         
         inline unsigned int row_to_y(unsigned int row) {
           return  m_render_const_factor*row+m_render_const_delta_y;
         }
         

        struct ZColor { int r; int g; int b; };

		ZColor get_net_color(ZNet* n) {
			if(!m_draw_rest_dimmed) 
				return m_net2color[n];

			ZColor z_color;
                        //changeSaturation(&z_color.r, &z_color.g,&z_color.b,0.5);
                        z_color.r = m_net2color[n].r/4;
                        z_color.g = m_net2color[n].g/4;
                        z_color.b = m_net2color[n].b/4;   

			return z_color;	
		}
		
         
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
            //std::cout << str << "->" << hash << " " << z_color.r << std::endl;
            
            ////std::cout << z_color.r << std::endl;
            ////std::cout << z_color.g << std::endl;
            ////std::cout << z_color.b << std::endl;
            ////std::cout << std::endl;
            
            return z_color;
         }
        */
         
         void pick_color_for_net(ZNet* n) {
            
            if ( m_net2color.find(n) == m_net2color.end() ) {
              ZColor z_color = get_rand_color();//hash_color(n->get_name()); //
              m_net2color[n]=z_color;
            }
            
            set_drawing_color(get_net_color(n).r,get_net_color(n).g,get_net_color(n).b);
        }

  private:
        //ZRender m_renderer;
		ZTerm* m_active_term;
        ZChannelRouter* m_router;
        std::map<ZNet*, ZColor> m_net2color;

        int m_render_const_factor;
        int m_render_const_delta;
		int m_render_const_delta_x;
		int m_render_const_delta_y;		 
        int m_render_multip;
        int m_term_shape_rect_size;
        int m_extension_point_radius;
		int m_highlight_thickness;
		bool m_draw_rest_dimmed;
		
        std::vector<ZTerm*> m_all_terms;

        bool m_metalmode;
};

#endif