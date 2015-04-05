#include <SDL/SDL.h>

#include "../includes/display.h"
#include "../includes/couleurs.h"


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
  if ((pixel = SDL_CreateRGBSurface(0, 5, 5, 32, 0, 0, 0, 0)) == NULL)
    return -1;
  if (SDL_FillRect(pixel, NULL, colors[display->color_index]) == -1)
    return -1;
  if (SDL_BlitSurface(pixel, NULL, display->screen, &pos) == -1)
    return -1;
  return SDL_Flip(display->screen);
}
