#include <SDL/SDL.h>

#include "../includes/display.h"
#include "../includes/couleurs.h"


int			print_gomme(t_display *display, void *param)
{
  SDL_MouseButtonEvent	*mouse;
  SDL_Surface		*pixel;
  SDL_Rect		pos;


  if (display->button == RELEASED)
    return (0);
  mouse = (SDL_MouseButtonEvent *)param;
  pos.x = mouse->x - 2;
  pos.y = mouse->y - 2;
  if ((pixel = SDL_CreateRGBSurface(0, 10, 10, 32, 0, 0, 0, 0)) == NULL)
    return -1;
  if (SDL_FillRect(pixel, NULL, 0x00ffffff) == -1)
    return -1;
  if (SDL_BlitSurface(pixel, NULL, display->screen, &pos) == -1)
    return -1;
  
  /*if (add_layer(&display->layers, display->screen, &pos_0) == -1)
	return -1;*/


  return SDL_Flip(display->screen);
}
