#include <unistd.h>
#include <stdlib.h>

#include <sys/wait.h>

#include "../includes/interface.h"
#include "../includes/display.h"

void		getColorFromPalette(t_display *display)
{
  printf("Switch to picked\n");
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
      printf("Back to action %d, color : %x\n", display->action, display->current_color);
    }
  return (0);
}
