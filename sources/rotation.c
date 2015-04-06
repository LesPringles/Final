#include "../includes/interface.h"

int rot(t_display *display)
{
	SDL_Surface *tmp = NULL;
	SDL_Rect rect;
    SDL_Event event; 
	tmp = rotozoomSurface(display->screen, 90, 1, 0);

	//display->screen = tmp;
	rect.x =  event.button.x - tmp->w / 2;
    rect.y =  event.button.y - tmp->h / 2;

	SDL_BlitSurface(tmp, NULL, display->screen, &rect); //On affiche la rotation de la surface image.
    SDL_FreeSurface(tmp);
	SDL_Flip(display->screen);

	/*if (SDL_BlitSurface(tmp, NULL, display->screen, NULL) == -1)
		return -1;

	if (SDL_Flip(display->screen) == -1)
		return -1;*/

	
	return 0;

}
