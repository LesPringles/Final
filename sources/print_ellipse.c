#include <math.h>

#include "../includes/display.h"
#include "../includes/couleurs.h"

void ellipse(t_display *display, int cx, int cy, int width, int height, Uint32 coul)
{

  int a2 = width * width;
  int b2 = height * height;
  int fa2 = 4 * a2, fb2 = 4 * b2;
  int x, y, sigma;

  if (!width || !height)
    return ;
  /* first half */
  for (x = 0, y = height, sigma = 2*b2+a2*(1-2*height); b2*x <= a2*y; x++)
    {
      setPixelVerif(display, cx + x, cy + y, coul);
      setPixelVerif(display, cx - x, cy + y, coul);
      setPixelVerif(display, cx + x, cy - y, coul);
      setPixelVerif(display, cx - x, cy - y, coul);
      if (sigma >= 0)
        {
	  sigma += fa2 * (1 - y);
	  y--;
        }
      sigma += b2 * ((4 * x) + 6);
    }

  /* second half */
  for (x = width, y = 0, sigma = 2*a2+b2*(1-2*width); a2*y <= b2*x; y++)
    {
      setPixelVerif(display, cx + x, cy + y, coul);
      setPixelVerif(display, cx - x, cy + y, coul);
      setPixelVerif(display, cx + x, cy - y, coul);
      setPixelVerif(display, cx - x, cy - y, coul);

      if (sigma >= 0)
        {
	  sigma += fb2 * (1 - x);
	  x--;
        }
      sigma += a2 * ((4 * y) + 6);
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

static int		display_ellipse(t_display *display, SDL_Rect *pos, int cx, int cy, int width, int height)
{
  SDL_Surface		*square;

  if ((square = SDL_CreateRGBSurface(0, 0, 0, 32, 0, 0, 0, 0)) == NULL)
    return -1;

  ellipse(display, cx, cy, width, height, display->current_color);

  if (SDL_BlitSurface(square, NULL, display->screen, pos) == -1)
    return -1;
  return SDL_Flip(display->screen);

  return 0;
}


static int		get_w(SDL_Rect *start, SDL_Rect *end)
{
  return abs(end->x - start->x);
}

static int		get_h(SDL_Rect *start, SDL_Rect *end)
{
  return abs(end->y - start->y);
}

int			print_ellipse(t_display *display, void *param)
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

  return display_ellipse(display,
			 ((pos.x > start_pos->x) ? &pos : start_pos),
			 start_pos->x,
			 start_pos->y,
			 get_w(start_pos, &pos),
			 get_h(start_pos, &pos));
}
