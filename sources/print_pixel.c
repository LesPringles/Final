#include <SDL/SDL.h>

#include "../includes/display.h"
#include "../includes/couleurs.h"


void disc(t_display *display, int cx, int cy, long rayon, Uint32 coul);


int			print_pixel_carre(t_display *display, void *param)
{
  static t_button_state	last_state = UNSET;
  SDL_MouseButtonEvent	*mouse;
  SDL_Surface		*pixel;
  SDL_Rect		pos;
  SDL_Rect		pos_0;

  memset(&pos_0, 0, sizeof(SDL_Rect));
  if (display->button == RELEASED && last_state == UNSET)
    return (0);
  mouse = (SDL_MouseButtonEvent *)param;
  pos.x = mouse->x - 2;
  pos.y = mouse->y - 2;
  if ((pixel = SDL_CreateRGBSurface(0, 5, 5, 32, 0, 0, 0, 0)) == NULL)
    return -1;
  if (SDL_FillRect(pixel, NULL, display->current_color) == -1)
    return -1;
  if (SDL_BlitSurface(pixel, NULL, display->screen, &pos) == -1)
    return -1;

  if (((last_state == UNSET) || (last_state == RELEASED))
      && (display->button == PRESSED))
    last_state = PRESSED;

  if (display->button == RELEASED)
    {
      if (add_layer(&display->layers, display->screen, &pos_0) == -1)
	return -1;
      last_state = UNSET;
    }
  return SDL_Flip(display->screen);
}

int			print_pixel_rond(t_display *display, void *param)
{
  static t_button_state	last_state = UNSET;
  SDL_MouseButtonEvent	*mouse;
  SDL_Surface		*pixel;
  SDL_Rect		pos;
  SDL_Rect		pos_0;

  memset(&pos_0, 0, sizeof(SDL_Rect));
  if (display->button == RELEASED && last_state == UNSET)
    return (0);
  mouse = (SDL_MouseButtonEvent *)param;
  pos.x = mouse->x - 2;
  pos.y = mouse->y - 2;
  if ((pixel = SDL_CreateRGBSurface(0, 0, 0, 32, 0, 0, 0, 0)) == NULL)
    return -1;

  if (((last_state == UNSET) || (last_state == RELEASED))
      && (display->button == PRESSED))
    last_state = PRESSED;

  disc(display, pos.x, pos.y, 3, display->current_color);

  if (display->button == RELEASED)
    {
      if (add_layer(&display->layers, display->screen, &pos_0) == -1)
	return -1;
      last_state = UNSET;
    }

  if (SDL_BlitSurface(pixel, NULL, display->screen, &pos) == -1)
    return -1;

  return SDL_Flip(display->screen);
}
