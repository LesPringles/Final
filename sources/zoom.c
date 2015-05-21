#include "../includes/interface.h"

void zoom_surface(SDL_Surface *picture,double zoom) 
{ 
	SDL_Surface *rotation = NULL;

	rotation = rotozoomSurface(picture, 0.0, zoom, 0); 

	new(picture);
	SDL_BlitSurface(rotation, NULL, picture, NULL); 
	SDL_FreeSurface(rotation); 
	SDL_Flip(picture);
	//correction rotation noir???
}


