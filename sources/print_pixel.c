#include <SDL/SDL.h>

#include "../includes/display.h"
#include "../includes/couleurs.h"


void disc(t_display *display, int cx, int cy, long rayon, Uint32 coul);


int			print_pixel(t_display *display, void *param)
{
  SDL_MouseButtonEvent	*mouse;
  SDL_Surface		*pixel;
  SDL_Rect		pos;



  if (display->button == RELEASED)
    return (0);
  mouse = (SDL_MouseButtonEvent *)param;
  pos.x = mouse->x - 2;
  pos.y = mouse->y - 2;
  if ((pixel = SDL_CreateRGBSurface(0, 0, 0, 32, 0, 0, 0, 0)) == NULL)
    return -1;

  disc(display, pos.x, pos.y, 5, colors[display->color_index]);

  if (SDL_BlitSurface(pixel, NULL, display->screen, &pos) == -1)
    return -1;
  
  /*if (add_layer(&display->layers, display->screen, &pos_0) == -1)
	return -1;*/


  return SDL_Flip(display->screen);
}
