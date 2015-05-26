#include <stdio.h>
#include <SDL/SDL.h>

#include "../includes/interface.h"

int		rotate_surface(t_display *display);

int 	Xrepereinit=0, Yrepereinit=0;
int 	nfractale=0;
int 	nfractalemax=4;
double 	parametreC[4*2]={0,0,-0.7927,0.1609,0.32,0.043,-0.0986,-0.65186};
double 	paramCx, paramCy;
//-Apparence de la fractale (Ces variables peuvent être modifiées)
int 	pas=10;
int 	facteur=1;

void 	fdessinfractale(t_display *display, int zoom, int Xrepereinit, int Yrepereinit, double paramCx, double paramCy);

int Xinit, Yinit;
double Xrepere, Yrepere, Zx, Zy, Cx, Cy, copie;
int n;
SDL_Rect position;



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
    case SDLK_t:
      fdessinfractale(display, 1, 0, 0, 0, 0);

	  {Xrepereinit=0;Yrepereinit=0;facteur=1;if(nfractale<=nfractalemax){nfractale=nfractale+2;}else{nfractale=0;};fdessinfractale(display, facteur,Xrepereinit,Yrepereinit,parametreC[nfractale],parametreC[nfractale+1]);};
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
