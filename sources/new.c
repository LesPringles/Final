#include <SDL/SDL.h>

#include "../includes/display.h"

int		new(SDL_Surface *surface)
{
  if (SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 255)) == -1)
    return -1;
  
	if (SDL_BlitSurface(surface, NULL, surface, NULL) == -1) 
		return -1; 
	return SDL_Flip(surface);
	return 0;
}
