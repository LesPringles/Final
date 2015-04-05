#include "../includes/display.h"
#include "../includes/couleurs.h"
#include <math.h>

void echangerEntiers(int* x, int* y)
{
  int t = *x;
  *x = *y;
  *y = t;
}

void ligne(t_display *display, int x1, int y1, int x2, int y2, Uint32 coul)
{
  int d, dx, dy, aincr, bincr, xincr, yincr, x, y;

  if (abs(x2 - x1) < abs(y2 - y1)) {
    /* parcours par l'axe vertical */

    if (y1 > y2) {
      echangerEntiers(&x1, &x2);
      echangerEntiers(&y1, &y2);
    }

    xincr = x2 > x1 ? 1 : -1;
    dy = y2 - y1;
    dx = abs(x2 - x1);
    d = 2 * dx - dy;
    aincr = 2 * (dx - dy);
    bincr = 2 * dx;
    x = x1;
    y = y1;

    setPixelVerif(display, x, y, coul);

    for (y = y1+1; y <= y2; ++y) {
      if (d >= 0) {
	x += xincr;
	d += aincr;
      } else
	d += bincr;

      setPixelVerif(display, x, y, coul);
    }

  } else {
    /* parcours par l'axe horizontal */

    if (x1 > x2) {
      echangerEntiers(&x1, &x2);
      echangerEntiers(&y1, &y2);
    }

    yincr = y2 > y1 ? 1 : -1;
    dx = x2 - x1;
    dy = abs(y2 - y1);
    d = 2 * dy - dx;
    aincr = 2 * (dy - dx);
    bincr = 2 * dy;
    x = x1;
    y = y1;

    setPixelVerif(display, x, y, coul);

    for (x = x1+1; x <= x2; ++x) {
      if (d >= 0) {
	y += yincr;
	d += aincr;
      } else
	d += bincr;

      setPixelVerif(display, x, y, coul);
    }
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

static int		display_ligne(t_display *display, SDL_Rect *pos, int x1, int y1, int x2, int y2)
{
  SDL_Surface		*square;

  if ((square = SDL_CreateRGBSurface(0, 0, 0, 32, 0, 0, 0, 0)) == NULL)
    return -1;

  ligne(display, x1, y1, x2, y2, colors[display->color_index]);

  if (SDL_BlitSurface(square, NULL, display->screen, pos) == -1)
    return -1;
  return SDL_Flip(display->screen);
  return 0;
}


int			print_ligne(t_display *display, void *param)
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
  //BEGIN APERCU
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
	//END APERCU
  if (display->button == RELEASED)
    {
	  //SAVE LAYER
	  if (last_state == PRESSED)
		if (add_layer(&display->layers, display->screen, &pos_0) == -1)
	  		return -1;
      last_state = RELEASED;
      original_screen = NULL;
      start_pos = NULL;
      return 0;
    }
	//BEGIN APERCU

  if (SDL_BlitSurface(original_screen, NULL, display->screen, &pos_0) == -1)
	    return -1;
	// END APERCU

		return display_ligne(display,
			((pos.x > start_pos->x) ? &pos : start_pos),
			mouse->x,
			mouse->y,
			start_pos->x,
			start_pos->y);


}
