#include <stdio.h>
#include <SDL/SDL.h>

#include "../includes/interface.h"

int		rotate_surface(t_display *display);



static int	manage_mouse_event(SDL_Event *event, t_display *display)
{
  (void)display;
  switch (event->type)
    {
    case SDL_MOUSEMOTION:
      return do_action(display, &event->button);
      break;
    case SDL_MOUSEBUTTONDOWN:
      display->button = PRESSED;
      return do_action(display, &event->button);
      break;
    case SDL_MOUSEBUTTONUP:
      display->button = RELEASED;
      break;
    default:
      break;
    }
  return 0;
}

static void	change_color(t_display *display)
{
  display->color_index++;
  if (display->color_index > NB_COLORS)
    display->color_index = 0;
  display->current_color = colors[display->color_index];
}


static int	manage_key_event(SDL_Event *event, t_display *display)
{
  (void)display;
  //unsigned int i = 0;

  switch ((int)event->key.keysym.sym)
    {
    case SDLK_ESCAPE:
      //return 1;
      break;
    case SDLK_SPACE:
      display->button = RELEASED;
      do_action(display, &event->button);
      if (display->action == 6)
		{
	  		display->action = -1;
		}
      display->action = display->action + 1;

      break;
    case SDLK_n:
      if (new(display->screen) == -1)
	return -1;
      break;
    case SDLK_BACKSPACE:
      if (undo(display) == -1)
	return -1; 
      break;
    case SDLK_RSHIFT:
      if (redo(display) == -1)
	return -1;
      break;
    case SDLK_s:
      if (save(display->screen, "ressources/out.bmp") == -1)
	return -1;
      break ;
    case SDLK_c:
      change_color(display);
      break;
	case SDLK_r:
	 if (new(display->screen) == -1)
		return -1;

	 if (rotate_surface(display) == -1)
		return -1;
	  break;

    default:
      break;
    }
  return 0;
}

static int	manage_event(SDL_Event *event, t_display *display)
{
  if (event->type == SDL_QUIT)
    return 1;
  if (event->type == SDL_KEYDOWN)
    return manage_key_event(event, display);
  return manage_mouse_event(event, display);
}

int	sdl_loop(t_display *display)
{
  SDL_Event	event;
  int		ret;

  display->current_color = colors[0];
  while (SDL_WaitEvent(&event))
    {
      if ((ret = manage_event(&event, display)) == -1)
	return -1;
      else if (ret)
	return 0;
    }
  return 0;
}
