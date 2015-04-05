#include <math.h>

#include "../includes/display.h"
#include "../includes/couleurs.h"

/*void setPixel(t_display *display, int x, int y, Uint32 coul)
{
  *((Uint32*)(display->screen->pixels) + x + y * WINX) = coul;
}


void setPixelVerif(t_display *display, int x, int y, Uint32 coul)
{
  if (x >= 0 && x < WINX &&
      y >= 0 && y < WINY)
    setPixel(display, x, y, coul);
}*/


void ligneHorizontale(t_display *display, int x, int y, int w, Uint32 coul)
{
  SDL_Rect r;
  r.x = x;
  r.y = y;
  r.w = w;
  r.h = 1;

  SDL_FillRect(display->screen, &r, coul);
}


void disc(t_display *display, int cx, int cy, long rayon, Uint32 coul)
{

	 int d, y, x;

  d = 3 - (2 * rayon);
  x = 0;
  y = rayon;

  while (y >= x) {
    ligneHorizontale(display, cx - x, cy - y, 2 * x + 1, coul);
    ligneHorizontale(display, cx - x, cy + y, 2 * x + 1, coul);
    ligneHorizontale(display, cx - y, cy - x, 2 * y + 1, coul);
    ligneHorizontale(display, cx - y, cy + x, 2 * y + 1, coul);

    if (d < 0)
      d = d + (4 * x) + 6;
    else {
      d = d + 4 * (x - y) + 10;
      y--;
    }

    x++;
  }

}

static int		set_start_pos(SDL_Rect **p, Uint16 x, Uint16 y)
{
  if ((*p = malloc(sizeof(SDL_Rect))) == NULL)
    return -1;
  (*p)->x = x;
  (*p)->y = y;
  return 0;
}

static int		display_disc(t_display *display, SDL_Rect *pos, int cx, int cy, long r)
{
  SDL_Surface		*square;

  if ((square = SDL_CreateRGBSurface(0, 0, 0, 32, 0, 0, 0, 0)) == NULL)
    return -1;

  disc(display, cx, cy, r, colors[display->color_index]);

  if (SDL_BlitSurface(square, NULL, display->screen, pos) == -1)
    return -1;
  return SDL_Flip(display->screen);

  return 0;
}

static long 			get_r(SDL_Rect *start, SDL_Rect *end)
{
	return sqrtl((end->x - start->x) * (end->x - start->x) + (end->y - start->y) * (end->y - start->y));
}

int			print_disc(t_display *display, void *param)
{
  static t_button_state	last_state = UNSET;
  static SDL_Rect	*start_pos = NULL;
  static SDL_Surface	*original_screen = NULL; // SAVE FOR APERCU
  SDL_MouseButtonEvent	*mouse;
  SDL_Rect		pos;
  SDL_Rect		pos_0;

  pos_0.x = 0;
  pos_0.y = 0;
  mouse = (SDL_MouseButtonEvent *)param;
  pos.x = mouse->x;
  pos.y = mouse->y;

  if (last_state == UNSET && display->button == RELEASED)
    return (0);

  if ((start_pos == NULL)
      && ((last_state == UNSET) || (last_state == RELEASED))
      && (display->button == PRESSED))
    {
      last_state = PRESSED;
	  if ((original_screen = SDL_CreateRGBSurface(0, WINX, WINY, 32, 0, 0, 0, 0)) == NULL)
	return -1;
      if (SDL_BlitSurface(display->screen, NULL, original_screen, &pos_0) == -1)
	return -1;
      if (set_start_pos(&start_pos, mouse->x, mouse->y) == -1)
	return -1;


    }


  if (display->button == RELEASED)
    {
	  // SAVE LAYER
      if (last_state == PRESSED)
		if (add_layer(&display->layers, display->screen, &pos_0) == -1)
	  		return -1;
      last_state = RELEASED;
      original_screen = NULL;
      start_pos = NULL;
      return 0;
    }

	  if (SDL_BlitSurface(original_screen, NULL, display->screen, &pos_0) == -1)
    return -1;
  // END APERCU


      return display_disc(display,
			    ((pos.x > start_pos->x) ? &pos : start_pos),
			    start_pos->x,
			    start_pos->y,
			    get_r(start_pos, &pos));
}
