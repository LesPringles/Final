#include <unistd.h>
#include <stdlib.h>

#include <sys/wait.h>

#include "../includes/interface.h"
#include "../includes/display.h"

void		getColorFromPalette(t_display *display, int displayPalette)
{
  SDL_Rect	pos;

  if (display->action == PICK_COLOR)
    return ;
  memset(&pos, 0, sizeof(SDL_Rect));
  display->old_action = display->action;
  display->action = PICK_COLOR;
  if (displayPalette)
    {
      add_layer(&display->layers, display->screen, &pos);
      if (SDL_BlitSurface(SDL_LoadBMP("./ressources/palette.bmp"),
			  NULL,
			  display->screen,
			  &pos) == -1)
	return ;
      SDL_Flip(display->screen);
    }
}

int	getColorFromSurface(t_display *display, void *param)
{
  SDL_MouseButtonEvent	*mouse;

  mouse = (SDL_MouseButtonEvent *)param;
  if (display->button == PRESSED)
    {
      display->current_color = *((Uint32*)(display->screen->pixels) + mouse->x + mouse->y * WINX);
      display->action = display->old_action;
      if (SDL_BlitSurface(display->layers->layer,
			  NULL,
			  display->screen,
			  display->layers->pos) == -1)
	return (-1);
      return SDL_Flip(display->screen);
    }
  return (0);
}
