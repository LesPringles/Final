#include <math.h>

#include "../includes/display.h"
#include "../includes/couleurs.h"

#define LEVEL 1

#define MAX(a, b) (((a)>(b))?(a):(b))
#define ABS(a) (((a)<0)?-(a):(a))
 
#define SQRT_3  1.732

void dot(t_display *display, int x, int y, Uint32 color)
{
    SDL_Rect r = {x, y, 2, 2};
	color = display->current_color;
    SDL_FillRect(display->screen, &r, color);
}

void drawLine(t_display *display, int x1, int y1, int x2, int y2, Uint32 color)
{
    double x = x1, y = y1;
    double inc_x = x2 - x1, inc_y = y2 - y1;
    int m = MAX(ABS(inc_x), ABS(inc_y));
    inc_x /= m;
    inc_y /= m;
	color = display->current_color;
 
    for(; m >= 0; m--)
    {
        dot(display, (unsigned int)x, (unsigned int)y, color);
        x += inc_x;
        y += inc_y;
    }
}

void etoiles(t_display *display, int x1, int y1, int x2, int y2, int level, Uint32 coul)
{

	    if(level > 0)
    {
        int x3 = (2*x1 + x2)/3;
        int y3 = (2*y1 + y2)/3;
        int x5 = (x1 + 2*x2)/3;
        int y5 = (y1 + 2*y2)/3;
        int x4 = x3 + (x5 - x3)/2 + (y5 - y3)*SQRT_3/2;
        int y4 = y3 - (x5 - x3)*SQRT_3/2 + (y5 - y3)/2;
        etoiles(display, x1, y1, x3, y3, level-1, display->current_color);
        etoiles(display, x3, y3, x4, y4, level-1, display->current_color);
        etoiles(display, x4, y4, x5, y5, level-1, display->current_color);
        etoiles(display, x5, y5, x2, y2, level-1, display->current_color);
    }
    else
    {
        drawLine(display, x1, y1, x2, y2, SDL_MapRGB(display->screen->format,
            255*coul/LEVEL, 255*level/LEVEL, 255 - 255*coul/LEVEL));
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

static int		display_etoiles(t_display *display, SDL_Rect *pos/*, int x1, int y1, int x2, int y2*/)
{
  SDL_Surface		*square;

  if ((square = SDL_CreateRGBSurface(0, 0, 0, 32, 0, 0, 0, 0)) == NULL)
    return -1;

  /*etoiles(display, WINX/2, 		WINY/8,			WINX*13/16,	WINY*17/24, LEVEL, display->current_color);
  etoiles(display, WINX*13/16, 	WINY*17/24, 	WINX*3/16, 	WINY*17/24, LEVEL, display->current_color);
  etoiles(display, WINX*3/16, 	WINY*17/24, 	WINX/2, 	WINY/8, LEVEL, display->current_color);*/

  etoiles(display, WINX/2, 		WINY/8,			WINX*13/16,	WINY*17/24, LEVEL, display->current_color);
  etoiles(display, WINX*13/16, 	WINY*17/24, 	WINX*3/16, 	WINY*17/24, LEVEL, display->current_color);
  etoiles(display, WINX*3/16, 	WINY*17/24, 	WINX/2, 	WINY/8, LEVEL, display->current_color);


  if (SDL_BlitSurface(square, NULL, display->screen, pos) == -1)
    return -1;
  return SDL_Flip(display->screen);

  return 0;
}


int			print_etoiles(t_display *display, void *param)
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


      return display_etoiles(display,
			    ((pos.x > start_pos->x) ? &pos : start_pos)/*,
			    start_pos->x,
			    start_pos->y,
				200,
				200*/);

}
