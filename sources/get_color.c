#include <unistd.h>
#include <stdlib.h>

#include <sys/wait.h>

#include "../includes/interface.h"
#include "../includes/display.h"

void		getColorFromPalette(t_display *display)
{
  display->old_action = display->action;
  display->action = PICK_COLOR;
}

int	getColorFromSurface(t_display *display, void *param)
{
  SDL_MouseButtonEvent	*mouse;

  mouse = (SDL_MouseButtonEvent *)param;
  if (display->button == PRESSED)
    {
      display->current_color = *((Uint32*)(display->screen->pixels) + mouse->x + mouse->y * WINX);
      display->action = display->old_action;
    }
  return (0);
}
